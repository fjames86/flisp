#!/bin/bash

fname=flisp-core.h

echo > ${fname}

echo "#ifndef LISP_H" >> ${fname}
echo "#define LISP_H" >> ${fname}

echo >> ${fname}

echo "char flisp_core_source[] = \"\\" >> ${fname}

awk '{print $0 "\\";}' *.lisp >> ${fname}

echo "\";" >> ${fname}
echo >> ${fname}

echo "#endif" >> ${fname}





