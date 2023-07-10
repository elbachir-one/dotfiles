" Name:         dark-meadow 
" Author:       jliu2179/cliuj
" Maintainer:   jliu2179/cliuj
" License:      MIT

highlight clear

if exists('syntax_on')
  syntax reset
endif

let g:colors_name = 'dark-meadow'
set background=dark

" Color palette
let s:clear           =  ['NONE', 'NONE']
let s:DeepSkyBlue3    =  ['31',   '#0087af']
let s:SpringGreen3    =  ['35',   '#00ff87']
let s:DeepPink4       =  ['89',   '#87005f']
let s:SkyBlue1        =  ['117',  '#87afff']
let s:MediumVioletRed =  ['126',  '#af0087']
let s:MediumOrchid1   =  ['171',  '#d75fff']
let s:Red1            =  ['196',  '#ff0000']
let s:Thistle1        =  ['225',  '#ffd7ff']
let s:MistyRose1      =  ['224',  '#ffd7ff']
let s:Grey11          =  ['232',  '#1c1c1c']
let s:Grey15          =  ['235',  '#262626']
let s:Grey23          =  ['237',  '#3a3a3a']
let s:Grey54          =  ['245',  '#8a8a8a']

" Text style 
let s:italic      =   'italic'
let s:bold        =   'bold'
let s:underline   =   'underline'
let s:none        =   'NONE'

" Helpec function to set up highlight executions
function! s:highlight(group, fg, bg, style)
  exec  "highlight "  . a:group
    \ . " ctermfg="   . a:fg[0]
    \ . " ctermbg="   . a:bg[0]
    \ . " cterm="     . a:style
    \ . " guifg="     . a:fg[1]
    \ . " guibg="     . a:bg[1]
    \ . " gui="       . a:style
endfunction

" Syntax highlighting groups
"
" For reference on what each group does, please refer to this:
" vimdoc.sourceforge.net/htmldoc/syntax.html
"
call s:highlight('Comment',         s:DeepSkyBlue3,     s:clear,  s:italic)
call s:highlight('Constant',        s:DeepSkyBlue3,     s:clear,    s:none)
call s:highlight('String',          s:SkyBlue1,         s:clear,  s:italic)
call s:highlight('Character',       s:SpringGreen3,     s:clear,    s:none)
call s:highlight('Number',          s:SkyBlue1,         s:clear,    s:none)
call s:highlight('Boolean',         s:SpringGreen3,     s:clear,    s:bold)
call s:highlight('Float',           s:SkyBlue1,         s:clear,    s:none)
call s:highlight('Identifier',      s:Thistle1,         s:clear,    s:none)
call s:highlight('Function',        s:DeepPink4,        s:clear,    s:bold)
call s:highlight('Statement',       s:SpringGreen3,     s:clear,    s:none)
call s:highlight('Conditional',     s:DeepPink4,        s:clear,    s:bold)
call s:highlight('Repeat',          s:MediumOrchid1,    s:clear,    s:bold)
call s:highlight('Label',           s:DeepPink4,        s:clear,    s:bold)
call s:highlight('Operator',        s:DeepPink4,        s:clear,    s:bold)
call s:highlight('Keyword',         s:SpringGreen3,     s:clear,    s:bold)
call s:highlight('Exception',       s:SpringGreen3,     s:clear,    s:bold)
call s:highlight('PreProc',         s:SkyBlue1,         s:clear,    s:none)
call s:highlight('Include',         s:DeepPink4,        s:clear,    s:none)
call s:highlight('Define',          s:MediumOrchid1,    s:clear,    s:none)
call s:highlight('Macro',           s:DeepPink4,        s:clear,    s:none)
call s:highlight('PreCondit',       s:SkyBlue1,         s:clear,    s:none)
call s:highlight('Type',            s:MediumOrchid1,    s:clear,    s:bold)
call s:highlight('StorageClass',    s:MediumVioletRed,  s:clear,    s:bold)
call s:highlight('Structure',       s:MediumOrchid1,    s:clear,    s:bold)
call s:highlight('Typedef',         s:MediumVioletRed,  s:clear,    s:bold)
call s:highlight('Special',         s:MediumOrchid1,    s:clear,    s:none)
call s:highlight('SpecialChar',     s:MediumOrchid1,    s:clear,    s:none)
call s:highlight('Tag',             s:DeepSkyBlue3,     s:clear,    s:none)
call s:highlight('Delimiter',       s:MediumVioletRed,  s:clear,    s:none)
call s:highlight('SpecialComment',  s:Grey54,           s:clear,    s:none)
call s:highlight('Debug',           s:Red1,             s:clear,    s:none)
call s:highlight('Underlined',      s:SkyBlue1,         s:clear,    s:bold)
call s:highlight('Ignore',          s:Grey54,           s:clear,    s:none)
call s:highlight('Error',           s:Red1,             s:clear,    s:bold)
call s:highlight('Todo',            s:MediumOrchid1,    s:clear,    s:bold)

" Interface highlighting
call s:highlight('Normal',          s:MistyRose1,       s:clear,     s:none)
call s:highlight('Visual',          s:clear,            s:Grey15,    s:none)
call s:highlight('Cursor',          s:Thistle1,         s:clear,     s:none)
call s:highlight('LineNr',          s:Thistle1,         s:clear,     s:none)
call s:highlight('CursorLine',      s:clear,            s:Grey15,    s:none)
call s:highlight('CursorLineNr',    s:clear,            s:DeepPink4, s:none)
call s:highlight('Pmenu',           s:clear,            s:Grey23,    s:none)
