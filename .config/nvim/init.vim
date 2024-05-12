highlight Comment cterm=italic
highlight CursorLine ctermbg=Black cterm=NONE
highlight CursorLineNr ctermbg=Black cterm=bold ctermfg=Green

nnoremap <leader>s :source ~/.config/nvim/init.vim<CR>

nnoremap <C-l> 		:vertical resize -2<CR>
nnoremap <C-k> 		:vertical resize +2<CR>
nmap J <C-w>h
nmap M <C-w>l

noremap <silent> l k
noremap <silent> k j
noremap <silent> m l
noremap <silent> j h

" Tab Stuff
nnoremap tn :tabnew<Space>
nnoremap tk :tabnext<cr>
nnoremap tj :tabprev<cr>
nnoremap th :tabfirst<cr>
nnoremap tl :tablast<cr>

"map <leader>c :!pdflatex % && !zathura %:p:r.pdf &<CR>
nnoremap <leader>c :w<CR>:!pdflatex % && zathura %:r.pdf & disown<CR>

inoremap jk <Esc>
inoremap kj <Esc>
inoremap <expr><TAB> pumvisible() ? "\<C-n>" : "\<TAB>"

vnoremap < <gv
vnoremap > >gv

:vmap <Tab> >
:vmap <S-Tab> <

map <F8> :setlocal spell! spelllang=en_gb<CR>

set title
set ruler
set showmatch
set ignorecase
set incsearch
set hlsearch
set bs=2
set nobackup
set scrolloff=10
set splitright
set cursorline
set cursorcolumn
set backupcopy=yes
set bg=dark
set shiftwidth=4
set diffopt+=iwhite
set autoindent
set smartindent
set go=a
set mouse=a
set nohlsearch
set clipboard+=unnamedplus
set nocompatible
set encoding=utf-8
set number relativenumber
set wildmode=longest,list,full
set list listchars=nbsp:¬,tab:»·,trail:·,extends:>
set undofile
set undodir=/tmp
set nobackup
set nowritebackup
set updatetime=300
set signcolumn=yes

nnoremap <silent> <F5> :%s/\s\+$//<cr>
nnoremap <C-t> :NERDTreeToggle<CR>
nnoremap <C-f> :Files<CR>
nnoremap <TAB> :bnext<CR>
nnoremap <C-s> :vsplit<CR>
nnoremap <C-b> :make<CR>
nnoremap <C-m> :MarkdownPreview<CR>
nnoremap <C-v> :ToggleTerm<CR>
nnoremap <C-x> :vsp term://go run %<CR>
nnoremap <C-g> :vsp term://gcc -o main main.c %<CR>
"nnoremap : <cmd>FineCmdline<CR>

vnoremap <C-c> "+y
map <C-v> "+v

let mapleader ="!"

" Auto Install Vim Plug
if ! filereadable(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim"'))
    echo "Downloading junegunn/vim-plug to manage plugins..."
    silent !mkdir -p ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/
    silent !curl "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim" > ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim
    autocmd VimEnter * PlugInstall
endif

" Plugins
call plug#begin(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.config}/nvim/plugged"'))

Plug 'lervag/vimtex'
Plug 'iamcco/markdown-preview.nvim', { 'do': 'cd app && npx --yes yarn install' }
Plug 'img-paste-devs/img-paste.vim'
Plug 'ryanoasis/vim-devicons'
Plug 'akinsho/toggleterm.nvim', {'tag' : '*'}
Plug 'ray-x/go.nvim'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'
Plug 'mattn/emmet-vim'
Plug 'dense-analysis/ale'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'preservim/nerdtree'
Plug 'junegunn/goyo.vim'
Plug 'jansedivy/jai.vim'
Plug 'bling/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'tpope/vim-commentary'
Plug 'ap/vim-css-color'
Plug 'tpope/vim-fireplace', { 'for': 'clojure' }
Plug 'rdnetto/YCM-Generator', { 'branch': 'stable' }
Plug 'nsf/gocode', { 'tag': 'v.20150303', 'rtp': 'vim' }
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'neoclide/coc.nvim'
"Plug 'tpope/vim-markdown'
Plug 'fcpg/vim-fahrenheit'

Plug 'godlygeek/tabular'
Plug 'plasticboy/vim-markdown'
Plug 'preservim/vim-pencil'

call plug#end()

lua require("toggleterm").setup()

" Disables automatic commenting on newline:
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" Goyo plugin makes text more readable when writing prose:
map <leader>g :Goyo \| set bg=light \| set linebreak<CR>

" Spell-check set to <leader>o, 'o' for 'orthography':
map <leader>o :setlocal spell! spelllang=en_us<CR>

" Nerd tree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
if has('nvim')
    let NERDTreeBookmarksFile = stdpath('data') . '/NERDTreeBookmarks'
else
    let NERDTreeBookmarksFile = '~/.vim' . '/NERDTreeBookmarks'
endif

" vimling:
nm <leader>d :call ToggleDeadKeys()<CR>
imap <leader>d <esc>:call ToggleDeadKeys()<CR>a
nm <leader>i :call ToggleIPA()<CR>
imap <leader>i <esc>:call ToggleIPA()<CR>a
nm <leader>q :call ToggleProse()<CR>

" Ensure files are read as what I want:
let g:vimwiki_ext2syntax = {'.Rmd': 'markdown', '.rmd': 'markdown','.md': 'markdown', '.markdown': 'markdown', '.mdown': 'markdown'}
map <leader>v :VimwikiIndex
let g:vimwiki_list = [{'path': '~/vimwiki', 'syntax': 'markdown', 'ext': '.md'}]

autocmd BufRead,BufNewFile /tmp/calcurse*,~/.calcurse/notes/* set filetype=markdown
autocmd BufRead,BufNewFile *.ms,*.me,*.mom,*.man set filetype=groff
autocmd BufRead,BufNewFile *.tex set filetype=tex

" Save file as sudo on files that require root permission
cnoremap w!! execute 'silent! write !sudo tee % >/dev/null' <bar> edit!

" Enable Goyo by default for mutt writing
autocmd BufRead,BufNewFile /tmp/neomutt* let g:goyo_width=80
autocmd BufRead,BufNewFile /tmp/neomutt* :Goyo | set bg=light
autocmd BufRead,BufNewFile /tmp/neomutt* map ZZ :Goyo\|x!<CR>
autocmd BufRead,BufNewFile /tmp/neomutt* map ZQ :Goyo\|q!<CR>

autocmd BufWritePre * %s/\s\+$//e
autocmd BufWritepre * %s/\n\+\%$//e
autocmd BufWritePost files,directories !shortcuts
autocmd BufWritePost *Xresources,*Xdefaults !xrdb %
autocmd BufWritePost *sxhkdrc !pkill -USR1 sxhkd

if &diff
    highlight! link DiffText MatchParen
endif

" Themes
colorscheme fahrenheit
"colorscheme orbital
"colorscheme gruvbox
"colorscheme tokyonight-storm
"colorscheme tokyonight-night
"colorscheme farout
"colorscheme PaperColor
"colorscheme github_*
"colorscheme ayu
"colorscheme molokai
"colorscheme kolor
"colorscheme codeblocks_dark
"colorscheme skittles_dark

" Vim Airline
let g:conoline_color_normal_light = "ctermbg=240"
let g:conoline_color_normal_nr_light = "ctermbg=240 ctermfg=white"
let g:conoline_color_insert_light = "ctermbg=black"
let g:conoline_color_insert_nr_light = "ctermbg=red"
let g:Powerline_symbols = "fancy"
let g:Powerline_symbols_override = {'BRANCH': "\Ue0a0", 'LINE': "\Ue0a1", 'RO': "\Ue0a2"}
let g:Powerline_symbols_override = {'BRANCH': "\Ue0a0", 'LINE': "\Ue0a1", 'RO': "\Ue0a2"}
let g:airline_powerline_fonts = 1
let g:airline_right_alt_sep = ''
let g:airline_right_sep = ''
let g:airline_left_alt_sep= ''
let g:airline_left_sep = ''

let g:airline_powerline_fonts = 1
if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif

" Unicode Symbols
let g:airline_left_sep = '»'
let g:airline_left_sep = '▶'
let g:airline_right_sep = '«'
let g:airline_right_sep = '◀'
let g:airline_symbols.linenr = '␊'
let g:airline_symbols.linenr = '␤'
let g:airline_symbols.linenr = '¶'
let g:airline_symbols.branch = '⎇'
let g:airline_symbols.paste = 'ρ'
let g:airline_symbols.paste = 'Þ'
let g:airline_symbols.paste = '∥'
let g:airline_symbols.whitespace = 'Ξ'

" Airline Themes
"let g:airline_theme='kolor'
"let g:airline_theme='dark'
"let g:airline_theme='badwolf'
let g:airline_theme='ravenpower'
"let g:airline_theme='simple'
"let g:airline_theme='term'
"let g:airline_theme='ubaryd'
"let g:airline_theme='laederon'
"let g:airline_theme='molokai'
"let g:airline_theme='powerlineish'

" Goyo setup
autocmd BufNewFile, BufRead *.vpm call SetVimPresentationMode()
function SetVimPresentationMode()
    nnoremap <buffer> <Right> :n<CR>
    nnoremap <buffer> <Left> :N<CR>

    if !exists('#goyo')
	Goyo
    endif
endfunction

" Coc

let g:coc_global_extensions = [
	    \ 'coc-tsserver',
	    \ 'coc-clangd'
	    \  ]
inoremap <silent><expr> <TAB>
	    \ coc#pum#visible() ? coc#pum#next(1):
	    \ CheckBackspace() ? "\<Tab>" :
	    \ coc#refresh()
inoremap <expr><S-TAB> coc#pum#visible() ? coc#pum#prev(1) : "\<C-h>"
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
	    \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"
function! CheckBackspace() abort
    let col = col('.') - 1
    return !col || getline('.')[col - 1]  =~# '\s'
endfunction
if has('nvim')
    inoremap <silent><expr> <c-space> coc#refresh()
else
    inoremap <silent><expr> <c-@> coc#refresh()
endif
nmap <silent> [g <Plug>(coc-diagnostic-prev)
nmap <silent> ]g <Plug>(coc-diagnostic-next)
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nnoremap <silent> K :call ShowDocumentation()<CR>
function! ShowDocumentation()
    if CocAction('hasProvider', 'hover')
	call CocActionAsync('doHover')
    else
	call feedkeys('K', 'in')
    endif
endfunction
autocmd CursorHold * silent call CocActionAsync('highlight')
nmap <leader>rn <Plug>(coc-rename)
xmap <leader>f  <Plug>(coc-format-selected)
nmap <leader>f  <Plug>(coc-format-selected)
augroup mygroup
    autocmd!
    autocmd FileType typescript,json setl formatexpr=CocAction('formatSelected')
    autocmd User CocJumpPlaceholder call CocActionAsync('showSignatureHelp')
augroup end
xmap <leader>a  <Plug>(coc-codeaction-selected)
nmap <leader>a  <Plug>(coc-codeaction-selected)

nmap <leader>ac  <Plug>(coc-codeaction)
nmap <leader>qf  <Plug>(coc-fix-current)

nmap <leader>cl  <Plug>(coc-codelens-action)

xmap if <Plug>(coc-funcobj-i)
omap if <Plug>(coc-funcobj-i)
xmap af <Plug>(coc-funcobj-a)
omap af <Plug>(coc-funcobj-a)
xmap ic <Plug>(coc-classobj-i)
omap ic <Plug>(coc-classobj-i)
xmap ac <Plug>(coc-classobj-a)
omap ac <Plug>(coc-classobj-a)

if has('nvim-0.4.0') || has('patch-8.2.0750')
    nnoremap <silent><nowait><expr> <C-d> coc#float#has_scroll() ? coc#float#scroll(1) : "\<C-d>"
    nnoremap <silent><nowait><expr> <C-b> coc#float#has_scroll() ? coc#float#scroll(0) : "\<C-b>"
    inoremap <silent><nowait><expr> <C-d> coc#float#has_scroll() ? "\<c-r>=coc#float#scroll(1)\<cr>" : "\<Right>"
    inoremap <silent><nowait><expr> <C-b> coc#float#has_scroll() ? "\<c-r>=coc#float#scroll(0)\<cr>" : "\<Left>"
    vnoremap <silent><nowait><expr> <C-d> coc#float#has_scroll() ? coc#float#scroll(1) : "\<C-d>"
    vnoremap <silent><nowait><expr> <C-b> coc#float#has_scroll() ? coc#float#scroll(0) : "\<C-b>"
endif

nmap <silent> <C-d> <Plug>(coc-range-select)
xmap <silent> <C-d> <Plug>(coc-range-select)

command! -nargs=0 Format :call CocActionAsync('format')

command! -nargs=? Fold :call     CocAction('fold', <f-args>)

command! -nargs=0 OR   :call     CocActionAsync('runCommand', 'editor.action.organizeImport')

set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}

nnoremap <silent><nowait> <space>a  :<C-u>CocList diagnostics<cr>
nnoremap <silent><nowait> <space>e  :<C-u>CocList extensions<cr>
nnoremap <silent><nowait> <space>c  :<C-u>CocList commands<cr>
nnoremap <silent><nowait> <space>o  :<C-u>CocList outline<cr>
nnoremap <silent><nowait> <space>s  :<C-u>CocList -I symbols<cr>
nnoremap <silent><nowait> <space>j  :<C-u>CocNext<CR>
nnoremap <silent><nowait> <space>k  :<C-u>CocPrev<CR>
nnoremap <silent><nowait> <space>p  :<C-u>CocListResume<CR>

" Markdown Preview
autocmd FileType markdown nmap <buffer><silent> <C-a> :call mdip#MarkdownClipboardImage()<CR>
" there are some defaults for image directory and image name, you can change them
" let g:mdip_imgdir = 'img'
" let g:mdip_imgname = 'image'
let g:mkdp_browser = 'qutebrowser'

" Emmet Stuff
let g:user_emmet_mode='n'
let g:user_emmet_leader_key=','

autocmd FileType markdown normal zR

augroup pencil
autocmd!
autocmd FileType markdown,mkd call pencil#init()
autocmd FileType text         call pencil#init()
augroup END
