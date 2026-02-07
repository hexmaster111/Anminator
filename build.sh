#!/bin/bash
set -x

cc main.c  -omain -ggdb -Wall -Wpedantic -lm -lraylib

