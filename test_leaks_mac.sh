#!/bin/bash

./server &
SERVER_PID=$!
sleep 1
echo "
Test [1]
"
./client $SERVER_PID "Este é um exemplo simples de texto gerado automaticamente com exatamente cem caracteres."
sleep 1
echo "
Test [2]
"
./client $SERVER_PID "Este é um exemplo simples de texto gerado automaticamente com exatamente cem caracteres.Este é um exemplo simples de texto gerado automaticamente com exatamente duzentos caracteres."
sleep 1
echo "
Test [3]
"
./client $SERVER_PID "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam rhoncus non dui ut hendrerit. Duis non ultrices turpis. Suspendisse posuere nec libero eget facilisis. Nunc egestas porta tempor. Quisque quis commodo magna. Quisque sit amet congue orci, sed interdum leo. Vivamus congue turpis sed ipsum tempus pellentesque. Morbi ac tortor laoreet, porta arcu id, gravida risus. Integer eget nisl vestibulum felis accumsan viverra ac sit amet ex.

Pellentesque dolor sapien, mollis vitae commodo sit amet, condimentum nec tortor. Maecenas ac nisl tincidunt, rhoncus mauris quis, faucibus nisl. Integer vulputate, turpis sed ornare imperdiet, urna nisl porttitor quam, quis porttitor mauris ipsum sed purus. Vivamus ornare porttitor diam ac iaculis. Nulla felis leo, rhoncus id mauris et, ultrices interdum dolor. Donec dapibus quis lectus eget gravida. Aliquam eu leo justo. Duis lacinia dui ut sem mollis imperdiet. Sed iaculis, ante sit amet ornare lacinia, metus risus tempus erat, quis auctor nisi.
"
sleep 1
leaks $SERVER_PID
kill $SERVER_PID
