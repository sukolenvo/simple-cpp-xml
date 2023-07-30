clang-format-check:
	find src test \( -name *.hpp -or -name *.cpp \) | xargs -I {} clang-format-14 --dry-run -Werror {}