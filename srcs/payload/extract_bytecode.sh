#!/bin/bash

script_dir=$(dirname $0)

# check whether or not we can display colors
if test -t 1; then
    ncolors=$(tput colors)
    if test -n "$ncolors" && test $ncolors -ge 8; then
        bold="$(tput bold)"
        reset="\033[0m"
        white="$(tput setaf 7)"
        red="$(tput setaf 1)"
        blue="$(tput setaf 4)"
        green="$(tput setaf 2)"
        gray="$(tput setaf 8)"
    fi
fi

echo -e "${bold}${blue}Searching text section:${reset}"

# Extract bytecode from the payload
text_section=$(readelf -S "$script_dir"/bytecode -W | grep .text)

if [ -z "$text_section" ]; then
    printf "${bold}${red}Error: ${white}No .text section found in the bytecode\n"
    exit 1
fi

echo -e "  ${bold}${green}.text section Found${reset}:\n${text_section}${reset}\n"

# Get the offset and size of the text section
offset=$(echo "$text_section" | awk '{print "0x"$6}')
size=$(echo "$text_section" | awk '{print "0x"$7}')

echo -e "${bold}${blue}Generating payload.c: ${gray}(offset: $offset, size: $size)${reset}"

# Extract the bytecode
xxd -s "$offset" -l "$size" -i "$script_dir"/bytecode > "$script_dir"/payload.c

if [ $? -ne 0 ]; then
  echo -e "  ${bold}${red}Error: ${white}Failed to extract bytecode${reset}"
  exit 1
fi

xxd_output=$(xxd -s "$offset" -l "$size" "$script_dir"/bytecode)

echo -e "  ${bold}${green}payload found${reset}:"

IFS=$'\n'
for line in $xxd_output; do
  addr=$(sed -E "s/^(.{9}).*$/\1/g" <<< "${line}")
  bytecode=$(sed -E "s/^.{10}(.{39}).*$/\1/g" <<< "${line}")
  memory=$(sed -E "s/^.{51}(.*)$/\1/g" <<< "${line}")

  echo -e "    ${gray}${addr}${reset}  ${bytecode}  ${gray}${memory}${reset}"
done

sed -i -E 's/^(unsigned).*(char|int)(.*)(\[\]|_len)/\1 \2 __bytecode\4/g' "$script_dir"/payload.c

echo -e "\n${bold}${green}Done${reset} ($script_dir/payload.c generated)"

