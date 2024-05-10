syntax on
set nu
set rnu
set tabstop=4
set incsearch
set hlsearch
set cursorline
"set cursorcolumn
set softtabstop=4
set lazyredraw
set mouse=a
set encoding=utf-8
set t_Co=256
set timeoutlen=50
set signcolumn=yes
set smarttab
set nobackup
set expandtab
set shiftwidth=4
set nohlsearch
set scrolloff=10
set undofile
set undodir=/tmp
set noswapfile
set ignorecase
set clipboard+=unnamedplus
set paste
set ai
set colorcolumn=80

"
hi CursorLine term=bold cterm=bold ctermbg=238
hi CursorLineNr term=none cterm=none ctermbg=238 ctermfg=1
hi CursorColumn ctermbg=238 ctermfg=230
hi SignColumn ctermbg=none cterm=bold term=bold
hi Search ctermfg=0 ctermbg=9 cterm=underline term=underline
hi VertSplit ctermbg=0 ctermfg=0
hi Visual ctermbg=14 ctermfg=232

vnoremap <C-c> "+y
map <C-v> "+v

let mapleader ="!"

nnoremap <C-n>      :tabnew<Space>
nnoremap <C-k> 		:vertical resize -2<CR>
nnoremap <C-o> 		:vertical resize +2<CR>
nnoremap <C-f>      :Files<CR>
nnoremap <TAB>      :bnext<CR>
nnoremap <C-s>      :vs<CR>

noremap m l
noremap j h
noremap k j
noremap l k

" PUG
if ! filereadable(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim"'))
    echo "Downloading junegunn/vim-plug to manage plugins..."
    silent !mkdir -p ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/
    silent !curl "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim" > ${XDG_CONFIG_HOME:-$HOME/.config}/nvim/autoload/plug.vim
    autocmd VimEnter * PlugInstall
endif

call plug#begin(system('echo -n "${XDG_CONFIG_HOME:-$HOME/.config}/nvim/plugged"'))

Plug 'junegunn/fzf.vim'
Plug 'dense-analysis/ale'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'img-paste-devs/img-paste.vim'
Plug 'iamcco/markdown-preview.nvim', { 'do': 'cd app && npx --yes yarn install' }
Plug 'SirVer/ultisnips'
Plug 'godlygeek/tabular'
Plug 'plasticboy/vim-markdown'
Plug 'preservim/vim-pencil'

call plug#end()

autocmd FileType gitdiff noremap m l
autocmd FileType gitdiff noremap j h
autocmd FileType gitdiff noremap k j
autocmd FileType gitdiff noremap l k

let g:mkdp_browser = 'qutebrowser'

autocmd FileType markdown nmap <buffer><silent> <C-a> :call mdip#MarkdownClipboardImage()<CR>
" there are some defaults for image directory and image name, you can change them
" let g:mdip_imgdir = 'img'
" let g:mdip_imgname = 'image'

autocmd FileType markdown normal zR

augroup pencil
autocmd!
autocmd FileType markdown,mkd call pencil#init()
autocmd FileType text         call pencil#init()
augroup END
