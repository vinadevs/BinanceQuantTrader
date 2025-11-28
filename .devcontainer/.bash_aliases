# -------------------------------
# Oh My Posh configuration
# -------------------------------
export PATH=$PATH:/root/.local/bin
eval "$(oh-my-posh init bash --config ~/.cache/oh-my-posh/themes/pure.omp.json)"

# -------------------------------
# fzf integration
# -------------------------------
source <(fzf --bash)
if [ ! -d /root/fzf-git ]; then
  git clone https://github.com/junegunn/fzf-git.sh.git /root/fzf-git && \
  chmod +x /root/fzf-git/fzf-git.sh
fi

source /root/fzf-git/fzf-git.sh

export FZF_CTRL_R_OPTS="
  --bind 'ctrl-y:execute-silent(echo -n {2..} | xclip -selection clipboard)+abort'
  --color header:italic
  --header 'Press CTRL-Y to copy command into clipboard'"

# -------------------------------
# fzf CTRL-T file preview
# -------------------------------
export FZF_CTRL_T_OPTS="--preview '(highlight -O ansi -l {} 2>/dev/null || cat {} || tree -C {}) 2>/dev/null | head -200'"
