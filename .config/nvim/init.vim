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
set diffopt+=iwhite
set autoindent
set smartindent
"set go=a
set mouse=a
set nohlsearch
set clipboard+=unnamedplus
set nocompatible
set encoding=utf-8
set number relativenumber
set wildmode=longest,list,full
set list
set listchars=nbsp:¬,tab:»·,trail:·,extends:>
set undofile
set undodir=/tmp
set nobackup
set nowritebackup
set updatetime=300
set signcolumn=yes
set noexpandtab
set tabstop=4
set softtabstop=4
set shiftwidth=4

highlight Comment cterm=italic
highlight CursorLine ctermbg=Black cterm=NONE
highlight CursorLineNr ctermbg=Black cterm=bold ctermfg=Green

let mapleader ="!"

noremap <silent> l k
noremap <silent> k j
noremap <silent> m l
noremap <silent> j h
nnoremap tl :tabnext<CR>
nnoremap tk :tabprev<CR>
nnoremap tj :tabfirst<CR>
nnoremap tm :tablast<CR>
nnoremap <leader>c :w<CR>:!pdflatex % && zathura %:r.pdf & disown<CR>
nnoremap <leader>s :source ~/.config/nvim/init.vim<CR>
nnoremap <C-n> :tabnew<Space>
nnoremap <C-k> :vertical resize -2<CR>
nnoremap <C-l> :vertical resize +2<CR>
nnoremap <silent> <F5> :%s/\s\+$//<cr>
nnoremap <C-x> :NERDTreeToggle<CR>
nnoremap <C-f> :Files<CR>
nnoremap <TAB> :bnext<CR>
nnoremap <C-s> :vs<CR>
nnoremap <C-c> :ba<CR>
nnoremap <C-b> :make<CR>
nnoremap <C-p> :MarkdownPreview<CR>
nnoremap <C-t> :ToggleTerm<CR>
nnoremap <C-g> :0Gclog<CR>
nnoremap J :bprev<CR>
nnoremap M :bnext<CR>
nnoremap L <C-u>
nnoremap K <C-d>

inoremap <expr><TAB> pumvisible() ? "\<C-n>" : "\<TAB>"

vnoremap < <gv
vnoremap > >gv
vnoremap <C-c> "+y

:vmap <Tab> >
:vmap <S-Tab> <

map <leader>o :setlocal spell! spelllang=en_us<CR>
map <leader>g :Goyo \| set bg=light \| set linebreak<CR>
map <F8> :setlocal spell! spelllang=en_gb<CR>
map <C-v> "+v

" Auto Install Vim Plug
if ! filereadable(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim"'))
	echo "Downloading junegunn/vim-plug to manage plugins..."
	silent !mkdir -p ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/
	silent !curl "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim" > ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim
	autocmd VimEnter * PlugInstall
endif

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
Plug 'fcpg/vim-fahrenheit'
Plug 'godlygeek/tabular'
Plug 'plasticboy/vim-markdown'
Plug 'preservim/vim-pencil'
Plug 'tpope/vim-fugitive'
Plug 'MeanderingProgrammer/render-markdown.nvim'
Plug 'lewis6991/gitsigns.nvim'

call plug#end()

lua require("toggleterm").setup()

" Disables automatic commenting on newline:
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

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
let g:airline_theme='ravenpower'

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
			\ 'coc-clangd',
			\ 'coc-html',
			\ 'coc-css',
			\ 'coc-yaml',
			\ 'coc-toml',
			\ 'coc-go',
			\ 'coc-pyright',
			\ 'coc-zig',
			\ 'coc-emmet',
			\ 'coc-lua',
			\ 'coc-texlab',
			\ 'coc-json',
			\ 'coc-sh',
			\ 'coc-eslint',
			\ 'coc-markdownlint'
			\  ]

inoremap <silent><expr> <TAB>
			\ coc#pum#visible() ? coc#pum#next(1) :
			\ CheckBackspace() ? "\<Tab>" :
			\ coc#refresh()

inoremap <expr><S-TAB> coc#pum#visible() ? coc#pum#prev(1) : "\<C-h>"

inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
			\: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

function! CheckBackspace() abort
	let col = col('.') - 1
	return !col || getline('.')[col - 1]  =~# '\s'
endfunction

inoremap <silent><expr> <c-space> coc#refresh()

" Markdown Preview
autocmd FileType markdown nmap <buffer><silent> <C-a> :call mdip#MarkdownClipboardImage()<CR>
" let g:mdip_imgdir = 'img'
" let g:mdip_imgname = 'image'
let g:mkdp_browser = 'qutebrowser'

let g:user_emmet_mode='n'
let g:user_emmet_leader_key=','

autocmd FileType markdown normal zR

augroup pencil
	autocmd!
	autocmd FileType markdown,mkd call pencil#init()
	autocmd FileType text         call pencil#init()
augroup END

autocmd BufWritePre *.c,*.cpp,*.h silent! execute '!clang-format -i %' | edit!
