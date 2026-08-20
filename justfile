mode := "debug"
build-dir := "build-" + mode
prefix := "/usr/local"
cpp-std := "c++26"

default:
    @just --list

configure m=mode install_prefix=prefix:
    #!/usr/bin/env bash
    set -euo pipefail
    args=(--buildtype={{ if m == "release" { "release" } else { "debug" } }} -Dcpp_std={{cpp-std}})
    [[ "{{m}}" == "release" ]] && args+=(-Db_lto=true)
    [[ "{{m}}" == "asan"    ]] && args+=(-Db_sanitize=address,undefined)
    if [[ -d "build-{{m}}" ]]; then
        meson setup "build-{{m}}" "${args[@]}" --prefix "{{install_prefix}}" --reconfigure
    else
        meson setup "build-{{m}}" "${args[@]}" --prefix "{{install_prefix}}"
    fi
    ln -sfn "build-{{m}}/compile_commands.json" compile_commands.json

_ensure-configured m=mode:
    #!/usr/bin/env bash
    set -euo pipefail
    if [[ ! -f "build-{{m}}/build.ninja" ]]; then
        just configure {{m}}
        exit 0
    fi
    configure_output="$(meson configure "build-{{m}}")"
    current_cpp_std="$(awk '$1 == "cpp_std" { print $2; found=1 } END { if (!found) exit 1 }' <<<"$configure_output")"
    args=()
    [[ "$current_cpp_std" != "{{cpp-std}}" ]] && args+=(-Dcpp_std={{cpp-std}})
    if (( ${#args[@]} > 0 )); then
        meson configure "build-{{m}}" "${args[@]}"
    fi

build m=mode: (_ensure-configured m)
    meson compile -C build-{{m}}

test m=mode: (_ensure-configured m)
    meson test -C build-{{m}} --print-errorlogs

run m=mode *args: (_ensure-configured m)
    meson compile -C build-{{m}}
    ./build-{{m}}/ping {{args}}

install m=mode: (_ensure-configured m)
    meson install -C build-{{m}}

clean m=mode:
    #!/usr/bin/env bash
    set -euo pipefail
    if [[ -L compile_commands.json && "$(readlink compile_commands.json)" == "build-{{m}}/compile_commands.json" ]]; then
        rm -f compile_commands.json
    fi
    rm -rf build-{{m}}

clean-all:
    rm -rf build-* compile_commands.json

rebuild m=mode: (clean m) (build m)

format:
    find src include tests -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) -exec clang-format -i {} +

tidy m=mode: (_ensure-configured m)
    clang-tidy -p build-{{m}} src/*.cpp
