" =============================================================================
" Name: vim-amber
" Amber theme for Vim
" -----------------------------------------------------------------------------
"
" This is a theme that simulates the retro PC amber monochrome CRT. It has
" pretty much no syntax-specific variations in color and is, instead, focusing
" on providing variations for the editor elements themselves (e.g., line
" numbers, color column, etc).
" 
" This theme can toggle between light and dark mode by setting the `background`
" variable to "light" or "dark".
"
" Some tricks and code borrowed from Lucious theme by Jonathan Filip.
" =============================================================================

highlight clear

if exists("syntax_on")
  syntax reset
endif

let g:colors_name="amber"

let s:everything = [
      \ "ColorColumn",
      \ "Comment",
      \ "Conceal",
      \ "Constant",
      \ "Cursor", 
      \ "CursorColumn",
      \ "CursorIM",
      \ "CursorLine",
      \ "CursorLineNr",
      \ "CursorLineNr",
      \ "DiffAdd", 
      \ "DiffChange",
      \ "DiffDelete",
      \ "DiffText",
      \ "Directory",
      \ "Error",
      \ "ErrorMsg", 
      \ "FoldColumn",
      \ "Folded", 
      \ "Identifier",
      \ "IncSearch",
      \ "LineNr",
      \ "MatchParen",
      \ "ModeMsg",
      \ "MoreMsg",
      \ "NonText",
      \ "Normal",
      \ "Pmenu",
      \ "PmenuSbar",
      \ "PmenuSel",
      \ "PmenuThumb",
      \ "PreProc",
      \ "Question",
      \ "Search",
      \ "SignColumn",
      \ "Special",
      \ "SpecialKey",
      \ "SpellBad", 
      \ "SpellCap",  
      \ "SpellLocal", 
      \ "SpellRare",
      \ "Statement",
      \ "StatusLine",
      \ "StatusLineNC",
      \ "TabLine",
      \ "TabLineFill",
      \ "TabLineSel",
      \ "Title",
      \ "Todo",
      \ "Type",
      \ "Underlined", 
      \ "VertSplit",
      \ "Visual",
      \ "VisualNOS",
      \ "WarningMsg",
      \ "WildMenu"
      \ ]

let s:inverted_items = [
      \ "Cursor",
      \ "CursorIM",
      \ "CursorColunmn",
      \ "CursorLineNr",
      \ "Visual", 
      \ "StatusLine"
      \ ]

let s:sub_inverted_items = [
      \ "VertSplit",
      \ "StatusLineNC",
      \ "PmenuSel"
      \ ]

let s:special_items = [
      \ "CursorLine",
      \ "ColorColumn",
      \ "NonText"
      \ ]

let s:error_items = [
      \ "SpellBad",
      \ "Error"
      \ ]

let s:style = &background

if s:style == "dark"
  let s:fg="#fc9505"
  let s:bg="#140b05"
  let s:special="#1c1008"
  let s:subbg="#c56306"
else
  let s:fg="#140b05"
  let s:bg="#fc9505"
  let s:special="#e58806"
  let s:subbg="#9e5d07"
endif

hi clear Normal

for s:item in s:everything
  exec "hi " . s:item . " guifg=" . s:fg . " guibg=" . s:bg .
        \ " gui=none ctermfg=NONE ctermbg=NONE cterm=none term=none"
endfor

for s:item in s:inverted_items
  exec "hi " . s:item . " guifg=" . s:bg . " guibg=" . s:fg 
endfor

for s:item in s:sub_inverted_items
  exec "hi " . s:item . " guifg=" . s:bg . " guibg=" . s:subbg 
endfor

for s:item in s:special_items
  exec "hi " . s:item . " guibg=" . s:special
endfor

for s:item in s:error_items
  exec "hi " . s:item . " guifg=#ff0000"
endfor
