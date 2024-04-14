# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of languages.
#
# The enumeration constants ${target}.language_(unknown|c|cpp|masm) are defined.
# Each constant is a string of an unique name identifying a language.
#
# @param target The target.
macro(define_languages target)
  # Unknown language.
  set(${target}.language_unknown "<unknown language>")
  # The "C" language.
  set(${target}.language_c "C")
  # The "C++" language.
  set(${target}.language_cpp "CPP")
  # The "MASM" language.
  set(${target}.language_masm "MASM")
endmacro()
