highlight Comment cterm=italic
highlight CursorLine ctermbg=Black cterm=NONE
highlight CursorLineNr ctermbg=Black cterm=bold ctermfg=Green

nnoremap <C-k> 		:vertical resize -2<CR>
nnoremap <C-o> 		:vertical resize +2<CR>
nnoremap <C-h>	<C-w>h
nnoremap <C-l>	<C-w>l

inoremap jk <Esc>
inoremap kj <Esc>
"
inoremap <expr><TAB> pumvisible() ? "\<C-n>" : "\<TAB>"

vnoremap < <gv
vnoremap > >gv

:vmap <Tab> >
:vmap <S-Tab> <

let g:user_emmet_mode='n'
let g:user_emmet_leader_key=','

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
set diffopt+=iwhite " Ignore whitespace whilst diffing
nnoremap <silent> <F5> :%s/\s\+$//<cr>

set list listchars=nbsp:¬,tab:»·,trail:·,extends:>

nnoremap <C-t> :NERDTreeToggle<CR>
nnoremap <C-f> :Files<CR>
nnoremap <TAB> :bnext<CR>
nnoremap <C-s> :vsplit<CR>

set undofile
set undodir=/tmp

vnoremap <C-c> "+y
map <C-v> "+v

let mapleader ="!"

call plug#begin()

Plug 'ryanoasis/vim-devicons'
Plug 'akinsho/toggleterm.nvim', {'tag' : '*'}
Plug 'NLKNguyen/papercolor-theme'
Plug 'ray-x/go.nvim'
Plug 'folke/tokyonight.nvim', { 'branch': 'main' }
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
Plug 'tpope/vim-fireplace', { 'for': 'clojure' }
Plug 'rdnetto/YCM-Generator', { 'branch': 'stable' }
Plug 'nsf/gocode', { 'tag': 'v.20150303', 'rtp': 'vim' }
Plug 'junegunn/fzf', { 'dir': '~/.fzf', 'do': './install --all' }
Plug 'tpope/vim-markdown'
Plug 'fcpg/vim-farout'
Plug 'fcpg/vim-fahrenheit'

call plug#end()

lua require("toggleterm").setup()

nnoremap <C-v> :ToggleTerm<CR>
nnoremap <C-x> :vsp term://go run %<CR>
nnoremap <C-g> :vsp term://gcc -o main main.c %<CR>

set go=a
set mouse=a
set nohlsearch
set clipboard+=unnamedplus

"tab stuff
    nnoremap tn :tabnew<Space>
    nnoremap tk :tabnext<cr>
    nnoremap tj :tabprev<cr>
    nnoremap th :tabfirst<cr>
    nnoremap tl :tablast<cr>
"Themes
colorscheme fahrenheit
"colorscheme gruvbox
"colorscheme tokyonight-storm
"colorscheme tokyonight-night
"colorscheme farout
"colorscheme PaperColor
"colorscheme github_*
" colorscheme ayu
"colorscheme molokai
"colorscheme kolor
"colorscheme codeblocks_dark
"colorscheme skittles_dark

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
" air-line
let g:airline_powerline_fonts = 1
if !exists('g:airline_symbols')
let g:airline_symbols = {}
endif
" unicode symbols
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
"
" Goyo setup
autocmd BufNewFile, BufRead *.vpm call SetVimPresentationMode()
function SetVimPresentationMode()
    nnoremap <buffer> <Right> :n<CR>
    nnoremap <buffer> <Left> :N<CR>

    if !exists('#goyo')
	    Goyo
    endif
endfunction
