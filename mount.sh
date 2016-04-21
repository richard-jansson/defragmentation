#!/bin/bash
mkdir mount 
sudo mount -o loop,uid=1000,gid=1000,umask=022 image mount
