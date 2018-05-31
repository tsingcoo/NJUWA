#!/bin/bash

LOCAL_REPO=`pwd`  # 设置本地项目地址
# mkdir -p $LOCAL_REPO/{dir1,dir2}
# touch $LOCAL_REPO/tmp1.txt
# touch $LOCAL_REPO/tmp2.txt

REMOTE_REPO_DIR="wangql@114.212.189.62:/home/wangql" # 设置远程项目parent目录
#PORT=32768  # 设置机器port

echo "==== LocalRepo [$LOCAL_REPO] -> RemoteRepo [$REMOTE_REPO_DIR/`basename $LOCAL_REPO`]"

rsync --delete -azvP -e "ssh" --exclude={.DS_Store,dir1} $LOCAL_REPO $REMOTE_REPO_DIR
fswatch -or $LOCAL_REPO | while read f; do
	rsync --delete -azvP -e "ssh" --exclude={.DS_Store,dir1} $LOCAL_REPO $REMOTE_REPO_DIR
done
