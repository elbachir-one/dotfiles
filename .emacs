(setq inhibit-startup-screen t)
(menu-bar-mode 0)

;; Map Escape key to Meta.
(setq x-meta-keysym 'escape)
(setq x-alt-keysym 'meta)

(global-set-key (kbd "M-x") 'smex)
(global-set-key (kbd "M-X") 'smex-major-mode-commands)
;; The old M-x.
(global-set-key (kbd "C-c C-c M-x") 'execute-extended-command)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(custom-enabled-themes '(modus-vivendi))
 '(package-selected-packages '(smex)))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
(setq backup-directory-alist '(("." . "~/.emacs_saves")))
(ido-mode t)
;; Adding melpa.
(package-initialize)
(add-to-list 'package-archives
	     '("melpa" . "https://melpa.org/packages/"))
