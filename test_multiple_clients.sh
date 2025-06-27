#!/bin/bash

echo "=== Teste de Múltiplos Clientes Sequenciais ==="

# Compila o projeto
echo "1. Compilando..."
make clean && make

if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    exit 1
fi

# Inicia o servidor
echo "2. Iniciando servidor..."
./server &
SERVER_PID=$!
sleep 1

# Obtém o PID do servidor minitalk
MINITALK_PID=$(ps aux | grep "./server" | grep -v grep | awk '{print $2}' | head -1)

if [ -z "$MINITALK_PID" ]; then
    echo "Erro: Servidor não iniciado!"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi

echo "Servidor rodando com PID: $MINITALK_PID"

# Teste com múltiplos clientes enviando simultaneamente
echo "3. Testando múltiplos clientes simultaneamente..."
echo "   Cada cliente envia uma mensagem diferente"

./client $MINITALK_PID "Mensagem do Cliente 1" &
CLIENT1_PID=$!

./client $MINITALK_PID "Mensagem do Cliente 2 - Esta é mais longa para testar" &
CLIENT2_PID=$!

./client $MINITALK_PID "Cliente 3" &
CLIENT3_PID=$!

./client $MINITALK_PID "Último cliente enviando mensagem final" &
CLIENT4_PID=$!

# Espera todos os clientes terminarem
wait $CLIENT1_PID
wait $CLIENT2_PID  
wait $CLIENT3_PID
wait $CLIENT4_PID

echo "4. Aguardando processamento das mensagens..."
sleep 2

echo "5. Testando com intervalo entre envios..."
./client $MINITALK_PID "Primeira mensagem com intervalo"
sleep 1
./client $MINITALK_PID "Segunda mensagem com intervalo"
sleep 1
./client $MINITALK_PID "Terceira mensagem com intervalo"

echo "6. Finalizando servidor..."
kill $SERVER_PID 2>/dev/null
sleep 1

echo "=== Teste concluído ==="
echo "As mensagens devem aparecer sequencialmente, uma por vez, sem misturar"
