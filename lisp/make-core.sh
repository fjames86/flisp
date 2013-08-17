#!/bin/bash

fname=flisp-core.c

echo > ${fname}

echo >> ${fname}

echo "char flisp_core_source[] = \"\\" >> ${fname}

awk '{gsub("\"", "\\\""); print $0 "\\n\\";}' *.lisp >> ${fname}

echo "\";" >> ${fname}
echo >> ${fname}









