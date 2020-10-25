## str::capitalize
Return a copy of the string with its first character capitalized and the rest
lowercased.

## str::casefold
Return a casefolded copy of the string. Casefolded strings may be used for
caseless matching.

Casefolding is similar to lowercasing but more aggressive because it is intended
to remove all case distinctions in a string. For example, the German lowercase
letter 'ß' is equivalent to "ss". Since it is already lowercase, lower() would
do nothing to 'ß'; casefold() converts it to "ss".

## str::center
Return centered in a string of length width. Padding is done using the specified
fillchar (default is an ASCII space). The original string is returned if width
is less than or equal to s.len().

## str::count
Return the number of non-overlapping occurrences of substring sub in the
range [start, end]. Optional arguments start and end are interpreted as in
slice notation.

## str::empty

## str::encode

## str::endswith
Return True if the string ends with the specified suffix, otherwise return
False. suffix can also be a tuple of suffixes to look for. With optional start,
test beginning at that position. With optional end, stop comparing at that
position.

## str::expandtabs

## str::equal

## str::find
Return the lowest index in the string where substring sub is found within the
slice s[start:end]. Optional arguments start and end are interpreted as in slice
notation. Return -1 if sub is not found.

## str::isalnum
## str::isalpha
## str::isascii
## str::isdecimal
## str::isdigit
## str::islower
## str::isnumeric
## str::isprintable
## str::isspace
## str::istitle
## str::isupper
## str::join
## str::len
## str::ljust
## str::lower
## str::lstrip
## str::removeprefix
## str::removesuffix
## str::replace
## str::rfind
## str::rjust
## str::rsplit
## str::rstrip
## str::split
## str::splitlines
## str::sprintf
## str::startswith
## str::strip
## str::substr
## str::swapcase
## str::title
## str::upper
## str::vsprintf
## str::zfill
