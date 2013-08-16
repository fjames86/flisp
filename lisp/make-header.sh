#!/bin/bash

fname=flisp-core.h

echo > ${fname}

echo "#ifndef LISP_H" >> ${fname}
echo "#define LISP_H" >> ${fname}

echo >> ${fname}

echo "static char flisp_core_source[] = \"\\" >> ${fname}

awk '{gsub("\"", "\\\""); print $0 "\\n\\";}' *.lisp >> ${fname}

echo "\";" >> ${fname}
echo >> ${fname}

echo "#endif" >> ${fname}





