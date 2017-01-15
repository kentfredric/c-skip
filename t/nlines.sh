input() { 
  cat <<INPUT
hello
world
test
INPUT
}

expected() {
  cat <<EXPECTED
world
test
EXPECTED
}

diff --label got --label expected -Naur <( ./skip -n1 <(input) ) <( expected )
