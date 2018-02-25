#!/bin/bash
COMMENT=$1
git add --all
git commit -m "$COMMENT"
git push

chmod -R ug+rw ./*
