#!/bin/bash
rsync -av --no-perms --delete --inplace \
  --exclude='.*' \
  --exclude='compile_flags.txt' \
  --exclude='compile_commands.json' \
  --exclude='*.sh' \
  ./ "/run/user/1004/gvfs/smb-share:server=192.168.122.102,share=shareddocs/Documents/Scoundrel/"
