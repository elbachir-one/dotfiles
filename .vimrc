" General Settings
syntax on
set nu                      " Show line numbers
set rnu                     " Show relative line numbers
set tabstop=4               " Number of spaces that a <Tab> in the file counts for
set softtabstop=4           " Number of spaces that a <Tab> counts for while editing
set shiftwidth=4            " Number of spaces to use for each step of (auto)indent
set expandtab               " Use spaces instead of tabs
set smarttab                " Insert tabs on the start of a line according to shiftwidth
set incsearch               " Incremental search
set hlsearch                " Highlight search results
set nohlsearch              " Disable persistent search highlight
set cursorline              " Highlight the current line
" set cursorcolumn          " Highlight the current column (uncomment if needed)
set scrolloff=10            " Keep 10 lines visible above and below the cursor
set mouse=a                 " Enable mouse support
set encoding=utf-8          " Set default encoding to UTF-8
set t_Co=256                " Use 256 colors
set timeoutlen=50           " Time to wait for a mapped sequence to complete
set signcolumn=yes          " Always show the sign column
set lazyredraw              " Redraw only when we need to
set ignorecase              " Ignore case when searching
set cb=unnamedplus          " Use the clipboard for all operations
set autoindent              " Copy indent from current line when starting a new line
set ai                      " Auto-indent new lines
set colorcolumn=80          " Highlight column 80
set updatetime=300          " Faster completion
set nobackup                " Do not create backup files
set nowritebackup           " Do not create a backup before overwriting a file
set noswapfile              " Do not use swap files
set undofile                " Enable persistent undo
set undodir=/tmp            " Directory for undo files

" Visual Enhancements
hi CursorLine term=bold cterm=bold ctermbg=238
hi CursorLineNr term=none cterm=none ctermbg=238 ctermfg=1
hi CursorColumn ctermbg=238 ctermfg=230
hi SignColumn ctermbg=none cterm=bold term=bold
hi Search ctermfg=0 ctermbg=9 cterm=underline term=underline
hi VertSplit ctermbg=0 ctermfg=0
hi Visual ctermbg=14 ctermfg=232

" Clipboard and Navigation Key Mappings
vnoremap <C-c> "+y          " Copy to clipboard in visual mode
map <C-v> "+v               " Paste from clipboard

" Custom Key Mappings
let mapleader ="!"
nnoremap <C-n> :tabnew<Space>
nnoremap <C-k> :vertical resize -2<CR>
nnoremap <C-o> :vertical resize +2<CR>
nnoremap <C-f> :Files<CR>
nnoremap <TAB> :bnext<CR>
nnoremap <C-s> :vs<CR>
nnoremap <C-c> :ba<CR>
nnoremap <C-t> :term<CR>
nnoremap <C-m> :MarkdownPreview<CR>

" Vim Movement Customization
noremap m l
noremap j h
noremap k j
noremap l k

" Plugin Management with vim-plug
call plug#begin()
Plug 'junegunn/fzf.vim'
Plug 'dense-analysis/ale'
Plug 'jiangmiao/auto-pairs'
Plug 'junegunn/vim-easy-align'
Plug 'tpope/vim-surround'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'img-paste-devs/img-paste.vim'
Plug 'iamcco/markdown-preview.nvim', { 'do': 'cd app && npx --yes yarn install' }
Plug 'godlygeek/tabular'
Plug 'plasticboy/vim-markdown'
Plug 'preservim/vim-pencil'
Plug 'neoclide/coc.nvim'
call plug#end()

" Markdown and Gitdiff Specific Settings
autocmd FileType gitdiff noremap m l
autocmd FileType gitdiff noremap j h
autocmd FileType gitdiff noremap k j
autocmd FileType gitdiff noremap l k

let g:mkdp_browser = 'qutebrowser'
autocmd FileType markdown nmap <buffer><silent> <C-a> :call mdip#MarkdownClipboardImage()<CR>
autocmd FileType markdown normal zR

augroup pencil
    autocmd!
    autocmd FileType markdown,mkd call pencil#init()
    autocmd FileType text call pencil#init()
augroup END

" CoC (Conqueror of Completion) Configuration
inoremap <silent><expr> <TAB> coc#pum#visible() ? coc#pum#next(1) : CheckBackspace() ? "\<Tab>" : coc#refresh()
inoremap <expr><S-TAB> coc#pum#visible() ? coc#pum#prev(1) : "\<C-h>"
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm() : "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

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

" GoTo code navigation
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

" Show documentation in preview window
nnoremap <silent> K :call ShowDocumentation()<CR>
function! ShowDocumentation()
    if CocAction('hasProvider', 'hover')
        call CocActionAsync('doHover')
    else
        call feedkeys('K', 'in')
    endif
endfunction

" Highlight symbol and references when holding the cursor
autocmd CursorHold * silent call CocActionAsync('highlight')

" Symbol renaming
nmap <leader>rn <Plug>(coc-rename)

" Formatting selected code
xmap <leader>f  <Plug>(coc-format-selected)
nmap <leader>f  <Plug>(coc-format-selected)

augroup mygroup
    autocmd!
    " Setup formatexpr for specific file types
    autocmd FileType typescript,json setl formatexpr=CocAction('formatSelected')
    " Update signature help on jump placeholder
    autocmd User CocJumpPlaceholder call CocActionAsync('showSignatureHelp')
augroup end

" Applying code actions to the selected code block
xmap <leader>a  <Plug>(coc-codeaction-selected)
nmap <leader>a  <Plug>(coc-codeaction-selected)

" Remap keys for applying code actions at the cursor position
nmap <leader>ac  <Plug>(coc-codeaction-cursor)
nmap <leader>as  <Plug>(coc-codeaction-source)
nmap <leader>qf  <Plug>(coc-fix-current)

" Remap keys for applying refactor code actions
nmap <silent> <leader>re <Plug>(coc-codeaction-refactor)
xmap <silent> <leader>r  <Plug>(coc-codeaction-refactor-selected)
nmap <silent> <leader>r  <Plug>(coc-codeaction-refactor-selected)

" Run the Code Lens action on the current line
nmap <leader>cl  <Plug>(coc-codelens-action)

" Map function and class text objects
xmap if <Plug>(coc-funcobj-i)
omap if <Plug>(coc-funcobj-i)
xmap af <Plug>(coc-funcobj-a)
omap af <Plug>(coc-funcobj-a)
xmap ic <Plug>(coc-classobj-i)
omap ic <Plug>(coc-classobj-i)
xmap ac <Plug>(coc-classobj-a)
omap ac <Plug>(coc-classobj-a)

" Use native statusline support
set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}

" Mappings for CoCList
nnoremap <silent><nowait> <space>a :<C-u>CocList diagnostics<cr>
nnoremap <silent><nowait> <space>e :<C-u>CocList extensions<cr>
nnoremap <silent><nowait> <space>c :<C-u>CocList commands<cr>
nnoremap <silent><nowait> <space>o :<C-u>CocList outline<cr>
nnoremap <silent><nowait> <space>s :<C-u>CocList -I symbols<cr>
nnoremap <silent><nowait> <space>j :<C-u>CocNext<CR>
nnoremap <silent><nowait> <space>k :<C-u>CocPrev<CR>
nnoremap <silent><nowait> <space>p :<C-u>CocListResume<CR>
