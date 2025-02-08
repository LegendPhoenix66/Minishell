#!/bin/bash

# 1️⃣ Fichier de suppression readline plus complet
cat > readline.supp << 'EOF'
{
   readline_all
   Memcheck:Leak
   match-leak-kinds: all
   ...
   obj:*/libreadline.so.*
}
{
   readline_history
   Memcheck:Leak
   match-leak-kinds: all
   ...
   fun:add_history
}
{
   readline_terminal
   Memcheck:Leak
   match-leak-kinds: all
   ...
   fun:_rl_init_terminal_io
}
{
   readline_keymap
   Memcheck:Leak
   match-leak-kinds: all
   ...
   fun:rl_make_bare_keymap
}
{
   ncurses_tinfo
   Memcheck:Leak
   match-leak-kinds: all
   ...
   obj:*/libtinfo.so.*
}
EOF

# 🎨 Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'
BLUE='\033[0;34m'

# 📝 Fonction de test améliorée
run_memory_test() {
    local test_command="$1"
    local test_name="$2"

    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    echo "Command: $test_command"

    # Fichier temporaire pour les commandes
    echo "$test_command" > temp_commands.txt
    echo "exit" >> temp_commands.txt

    # Exécution avec Valgrind et analyse des résultats
    valgrind --leak-check=full \
             --show-leak-kinds=definite,indirect,possible \
             --track-origins=yes \
             --suppressions=readline.supp \
             --log-file="valgrind_${test_name}.log" \
             ./minishell < temp_commands.txt 2>/dev/null

    # Analyse améliorée des résultats
    local leaks=0
    if grep -q "definitely lost:" "valgrind_${test_name}.log"; then
        local definitely_lost=$(grep "definitely lost:" "valgrind_${test_name}.log" | awk '{print $4}')
        if [ "$definitely_lost" -gt 0 ]; then
            leaks=1
        fi
    fi

    if [ $leaks -eq 0 ]; then
        echo -e "${GREEN}✓ No memory leaks${NC}"
    else
        echo -e "${RED}✗ Memory leaks detected${NC}"
        # Afficher uniquement les fuites pertinentes
        grep -A 2 "definitely lost:" "valgrind_${test_name}.log"
    fi

    rm temp_commands.txt
    echo "----------------------------------------"
}

# 🧪 Fonction de rapport
generate_report() {
    echo -e "\n${BLUE}=== Memory Test Report ===${NC}"
    local total_tests=0
    local passed_tests=0

    for log in valgrind_*.log; do
        total_tests=$((total_tests + 1))
        if ! grep -q "definitely lost: [1-9]" "$log"; then
            passed_tests=$((passed_tests + 1))
        fi
    done

    echo "Tests passed: $passed_tests/$total_tests"
    if [ $passed_tests -eq $total_tests ]; then
        echo -e "${GREEN}All tests passed!${NC}"
    else
        echo -e "${RED}Some tests failed.${NC}"
    fi
}

# 🚀 Exécution des tests
echo -e "${BLUE}=== Starting Memory Tests ===${NC}"

# Tests basiques
run_memory_test "ls" "basic_ls"
run_memory_test "echo hello" "basic_echo"

# Tests redirections
run_memory_test "echo hello > test.txt" "redirection"
run_memory_test "cat < test.txt" "input_redirection"

# Tests pipes
run_memory_test "ls | grep a" "pipe"
run_memory_test "ls | grep a | wc -l" "multiple_pipes"

# Tests heredoc
run_memory_test "cat << EOF
hello
world
EOF" "heredoc"

# Tests builtins
run_memory_test "cd .." "cd_builtin"
run_memory_test "export TEST=123" "export_builtin"
run_memory_test "env" "env_builtin"

# Tests quotes
run_memory_test "echo 'hello world'" "single_quotes"
run_memory_test 'echo "hello world"' "double_quotes"

# 📊 Génération du rapport
generate_report

# 🧹 Nettoyage
rm -f readline.supp test.txt

echo -e "${BLUE}=== Tests Complete ===${NC}"
