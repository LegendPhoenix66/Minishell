#!/bin/bash

# 1️⃣ Création du fichier de suppression pour readline
cat > readline.supp << 'EOF'
{
   readline
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:readline
}
{
   readline_internal
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:rl_initialize
}
{
   add_history
   Memcheck:Leak
   match-leak-kinds: reachable
   ...
   fun:add_history
}
{
    readline_still_reachable
    Memcheck:Leak
    match-leak-kinds: reachable
    ...
    obj:*/libreadline.so.*
}
{
    readline_indirect
    Memcheck:Leak
    match-leak-kinds: indirect
    ...
    obj:*/libreadline.so.*
}
EOF

# 🎨 Couleurs pour le formatage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'
BLUE='\033[0;34m'

# 📝 Fonction de test avec suppressions readline
run_memory_test() {
    local test_command="$1"
    local test_name="$2"

    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    echo "Command: $test_command"

    # Créer fichier temporaire de commandes
    echo "$test_command" > temp_commands.txt
    echo "exit" >> temp_commands.txt

    # Exécuter avec Valgrind et suppressions
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --suppressions=readline.supp \
             --log-file="valgrind_${test_name}.log" \
             ./minishell < temp_commands.txt

    # Analyse des résultats (ignore les fuites readline)
    if grep -v "still reachable" "valgrind_${test_name}.log" | grep -q "definitely lost"; then
        echo -e "${RED}✗ Memory leaks detected (excluding readline)${NC}"
        echo "Check valgrind_${test_name}.log for details"
    else
        echo -e "${GREEN}✓ No memory leaks (excluding readline)${NC}"
    fi

    rm temp_commands.txt
    echo "----------------------------------------"
}

# 🧪 Tests
echo -e "${BLUE}=== Starting Memory Tests (Ignoring Readline Leaks) ===${NC}"

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

# 🧹 Nettoyage
rm -f readline.supp test.txt

echo -e "${BLUE}=== Tests Complete ===${NC}"
