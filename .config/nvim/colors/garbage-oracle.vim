  " Originally generated using themer https://github.com/mjswensen/themer


  if &background == 'dark'

  let s:shade0 = "#371d32" " Dark purple
  let s:shade1 = "#a3829c" " Mauve pink
  let s:shade2 = "#bdaaba" " Lightest mauve pink
  let s:shade3 = "#877184" " Darker mauve pink
  let s:shade4 = "#a28e9f" " Slightly lighter but darker mauve pink
  let s:shade5 = "#52394d" " Slightly lighter but still dark purple
  let s:shade6 = "#d7c6d6" " Light pink
  let s:shade7 = "#f2e2f1" " Lightest pink
  let s:accent0 = "#fb9495" " Light red
  let s:accent1 = "#bbbcf0" " Lilac (light purple)
  let s:accent2 = "#a9cdd6" " Light pale blue
  let s:accent3 = "#c1edcc" " Light pale green
  let s:accent4 = "#b0c0bc" " Pale olive
  let s:accent5 = "#19d3db" " Bright cyan
  let s:accent6 = "#eff2c0" " Pale yellow
  let s:accent7 = "#aafdb0" " Bright green

  endif



  if &background == 'light'

  let s:shade0 = "#f2e2f1" " Lightest pink
  let s:shade1 = "#7f4570" " Dark pink
  let s:shade2 = "#d3a1c5" " Mid tone pink
  let s:shade3 = "#c481af" " Slightly darker mid tone pink
  let s:shade4 = "#b5619a" " Darker than the above mid tone pink
  let s:shade5 = "#dbadcc" " Muted pink
  let s:shade6 = "#96206e" " Darker magenta
  let s:shade7 = "#870058"  " Dark pink
  let s:accent0 = "#a4303f" " Dark red
  let s:accent1 = "#2b5b75" " Dark teal
  let s:accent2 = "#734d90" " Dark purple
  let s:accent3 = "#376954" " Dark green
  let s:accent4 = "#3f59b7" " Dark blue
  let s:accent5 = "#684f54" " Dark brown
  let s:accent6 = "#5d5255" " Grey brown
  let s:accent7 = "#ffc6fd" " Red brown

  endif


  highlight clear
  syntax reset
  let g:colors_name = "garbage-oracle"

  """"""""""
  " Normal "
  """"""""""

  exec "hi Normal guifg=".s:shade6." guibg=".s:shade0

  " Not all terminals support italics properly. If yours does, opt-in.
  if !exists("g:garbage_terminal_italics")
          let g:garbage_terminal_italics = 0
  endif
  function! s:h(group, style)
          if g:garbage_terminal_italics == 0 && has_key(a:style, "cterm") && a:style["cterm"] == "italic"
                  unlet a:style.cterm
          endif
          execute "highlight" a:group
                                  \ "guifg="   (has_key(a:style, "fg")    ? a:style.fg.gui   : "NONE")
                                  \ "guibg="   (has_key(a:style, "bg")    ? a:style.bg.gui   : "NONE")
                                  \ "guisp="   (has_key(a:style, "sp")    ? a:style.sp.gui   : "NONE")
                                  \ "gui="     (has_key(a:style, "gui")   ? a:style.gui      : "NONE")
                                  \ "ctermfg=" . l:ctermfg
                                  \ "ctermbg=" . l:ctermbg
                                  \ "cterm="   (has_key(a:style, "cterm") ? a:style.cterm    : "NONE")
  endfunction

  """""""""""""""""
  " Syntax groups "
  """""""""""""""""

  " Default

  exec "hi Comment guifg=".s:shade1." gui=italic cterm=italic"
  exec "hi Constant guifg=".s:accent3
  exec "hi Character guifg=".s:accent4
  exec "hi Identifier guifg=".s:accent2." gui=none cterm=none"
  exec "hi Statement guifg=".s:accent5
  exec "hi PreProc guifg=".s:accent6
  exec "hi Type guifg=".s:accent6
  exec "hi Special guifg=".s:accent4
  exec "hi Underlined guifg=".s:accent5
  exec "hi Error guifg=".s:shade0." guibg=".s:shade1
  exec "hi Todo guifg=".s:accent0." guibg=".s:shade1

  " GitGutter

  exec "hi GitGutterAdd guifg=".s:accent3
  exec "hi GitGutterChange guifg=".s:accent7
  exec "hi GitGutterChangeDelete guifg=".s:accent2
  exec "hi GitGutterDelete guifg=".s:accent0

  " fugitive

  exec "hi gitcommitComment guifg=".s:shade3." gui=italic cterm=italic"
  exec "hi gitcommitOnBranch guifg=".s:shade3
  exec "hi gitcommitHeader guifg=".s:shade5
  exec "hi gitcommitHead guifg=".s:shade3
  exec "hi gitcommitSelectedType guifg=".s:accent3
  exec "hi gitcommitSelectedFile guifg=".s:accent3
  exec "hi gitcommitDiscardedType guifg=".s:accent2
  exec "hi gitcommitDiscardedFile guifg=".s:accent2
  exec "hi gitcommitUntrackedFile guifg=".s:accent0

  """""""""""""""""""""""
  " Highlighting Groups "
  """""""""""""""""""""""

  " Default

  exec "hi ColorColumn guibg=".s:shade5
  exec "hi Conceal guifg=".s:shade2
  exec "hi Cursor guifg=".s:shade0." guibg=".s:shade0
  exec "hi CursorColumn guifg=".s:shade0." guibg=".s:shade1
  exec "hi CursorLine guibg=".s:shade1." cterm=none"
  exec "hi Directory guifg=".s:accent5
  exec "hi DiffAdd guifg=".s:accent3." guibg=".s:shade1
  exec "hi DiffChange guifg=".s:accent2." guibg=".s:shade1
  exec "hi DiffDelete guifg=".s:accent0." guibg=".s:shade1
  exec "hi DiffText guifg=".s:accent2." guibg=".s:shade2
  exec "hi ErrorMsg guifg=".s:shade7." guibg=".s:accent0
  exec "hi VertSplit guifg=".s:shade0." guibg=".s:shade3
  exec "hi Folded guifg=".s:shade4." guibg=".s:shade1
  exec "hi FoldColumn guifg=".s:shade4." guibg=".s:shade1
  exec "hi SignColumn guibg=".s:shade0
  exec "hi IncSearch guifg=".s:shade0." guibg=".s:accent2
  exec "hi LineNr guifg=".s:shade2." guibg=".s:shade0
  exec "hi CursorLineNr guifg=".s:shade3." guibg=".s:shade1
  exec "hi MatchParen guibg=".s:shade2
  exec "hi MoreMsg guifg=".s:shade0." guibg=".s:accent4
  exec "hi NonText guifg=".s:shade2." guibg=".s:shade0
  exec "hi Pmenu guifg=".s:shade0." guibg=".s:shade2
  exec "hi PmenuSel guifg=".s:shade2." guibg=".s:shade0
  exec "hi PmenuSbar guifg=".s:accent3." guibg=".s:shade1
  exec "hi PmenuThumb guifg=".s:accent0." guibg=".s:shade2
  exec "hi Question guifg=".s:shade7." guibg=".s:shade1
  exec "hi Search guifg=".s:shade0." guibg=".s:accent2
  exec "hi SpecialKey guifg=".s:accent7." guibg=".s:shade0
  exec "hi SpellBad guifg=".s:accent0
  exec "hi SpellCap guifg=".s:accent2
  exec "hi SpellLocal guifg=".s:accent4
  exec "hi SpellRare guifg=".s:accent1
  exec "hi StatusLine guifg=".s:shade0." guibg=".s:shade1." gui=none cterm=none"
  exec "hi TabLine guifg=".s:shade5." guibg=".s:shade1
  exec "hi TabLineFill guibg=".s:shade0
  exec "hi TabLineSel guifg=".s:shade6." guibg=".s:shade0
  exec "hi Title guifg=".s:accent5
  exec "hi Visual guifg=".s:shade0." guibg=".s:shade1
  exec "hi VisualNOS guifg=".s:shade0." guibg=".s:shade1
  exec "hi WarningMsg guifg=".s:shade1
  exec "hi WildMenu guifg=".s:accent4." guibg=".s:shade1

  " NERDTree

  exec "hi NERDTreeExecFile guifg=".s:accent4
  exec "hi NERDTreeDirSlash guifg=".s:accent5
  exec "hi NERDTreeCWD guifg=".s:accent0

  """"""""""""
  " Clean up "
  """"""""""""

  unlet s:shade0 s:shade1 s:shade2 s:shade3 s:shade4 s:shade5 s:shade6 s:shade7 s:accent0 s:accent1 s:accent2 s:accent3 s:accent4 s:accent5 s:accent6 s:accent7
