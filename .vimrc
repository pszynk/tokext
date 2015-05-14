"tokex vim project config file


"alternative default ext
let g:alternateExtensions_h = "cc,cpp,CC,CPP,c"


let s:root_path = expand('<sfile>:p:h')
let s:build_paths = {'debug' : "Debug/"}
let s:exec_files = {'test' : "Debug/bin/tt.sh"}

let s:default_build = 'debug'
let s:default_exec = 'test'


au BufEnter cpp if !exists('b:start') | let b:start = s:Get(s:default_exec, s:exec_files) | endif

function! s:Get(key, dict)
    return join([s:root_path, a:dict[a:key]], "/")
endfunction

"function! Make(...)
"    let build_path_key = ""
"    let build_args = ""
"    if a:0 > 0
"        let build_path_key = a:1
"        let build_args = a:000[1:]
"    else
"        let build_path_key = s:default_build
"    endif

"    execute 'make' "-C" join([s:root_path, s:build_paths[build_path_key]], "/") build_args
"endfunction

"command! -nargs=* Make :call Make(<f-args>) | cwindow

