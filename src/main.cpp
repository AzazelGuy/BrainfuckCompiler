#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <fstream>

std::vector<char> memoria(30000, 0); // A fita com 30k de tamanho, zerada
size_t ptr = 0;                      // O ponteiro de dados

// Função mágica para mapear onde os loops abrem e fecham
std::map<size_t, size_t> mapear_loops(const std::string& codigo) {
    std::map<size_t, size_t> pares;
    std::stack<size_t> pilha;

    for (size_t i = 0; i < codigo.length(); ++i) {
        if (codigo[i] == '[') {
            pilha.push(i); //empura a posição para a pilha pelo topo
        } 
        else if (codigo[i] == ']') {
            if (!pilha.empty()) {
                size_t inicio = pilha.top(); //Salva posição do inicio olhando para o topo
                pilha.pop(); //remove o topo
                pares[inicio] = i; // Guarda o destino da ida
                pares[i] = inicio; // Guarda o destino da volta
            }
        }
    }
    return pares;
}

bool tem_extensao_bf(const std::string& nome_arquivo) {
    // Verifica se o nome tem pelo menos 3 caracteres (ex: a.bf)
    if (nome_arquivo.length() >= 3) {
        // Pega os 3 últimos caracteres da string
        std::string extensao = nome_arquivo.substr(nome_arquivo.length() - 3);
        return extensao == ".bf";
    }
    return false;
}

std::string ler_arquivo_brainfuck(const std::string& caminho_do_arquivo)
{
    std::ifstream arquivo(caminho_do_arquivo); //recebe o caminho do arquivo

    if (!arquivo.is_open()) //confere se ele SEQUER existe
    {
        std::cerr << "Couldn't find: " << caminho_do_arquivo << " check the file and try again" <<std::endl;
        exit(1);
    }

    std::string clean_codigo_brainfuck = ""; //códigolimpo
    char caractere;

    // Lê o arquivo caractere por caractere
    while (arquivo.get(caractere)) {
        // Filtra e só adiciona se for um dos 8 comandos do Brainfuck
        if (caractere == '>' || caractere == '<' || 
            caractere == '+' || caractere == '-' || 
            caractere == '.' || caractere == ',' || 
            caractere == '[' || caractere == ']') {
            clean_codigo_brainfuck += caractere;
        }
    }

    arquivo.close();
    return clean_codigo_brainfuck;
}


int main(int argc, char* argv[]) { //esse int them argumntos que pode ser inseridos por <nome doprograma>.exe <nome do arquivo>.bf
    std::cout << "Azazel's Brainfuck Compiler\n" << std::endl;
    //Garante a existencia do arquivo, ou melhor, do comando
    if (argc < 2) {
        std::cout << "Error: No file provided. Usage: " << argv[0] << " <file>.bf" << std::endl;
        return 1;
    }

    std::string caminho_arquivo = argv[1];

    // Garante que é um arquivo .bf
    if (!tem_extensao_bf(caminho_arquivo)) {
        std::cout << "Error: Invalid file extension. Please provide a valid <file>.bf" << std::endl;
        return 1;
    }

    // Passa o arquivo para a função de leitura que criamos acima
    std::string codigo_brainfuck = ler_arquivo_brainfuck(caminho_arquivo);

    // Mapeamos os loops antes de começar a rodar
    std::map<size_t, size_t> info_loops = mapear_loops(codigo_brainfuck);

    int size = codigo_brainfuck.size();

    // Usamos 'size_t' para i porque índices de strings e mapas não podem ser negativos
    for (size_t i = 0; i < size; i++) {
        
        // CORREÇÃO 1: Avaliar o caractere 'codigo_brainfuck[i]', não o índice 'i'
        switch (codigo_brainfuck[i]) {
            case '>':
                ptr += 1;
                break;
            case '<':
                ptr -= 1;
                break;
            case '+':
                memoria[ptr] += 1;
                break; 
            case '-':
                memoria[ptr] -= 1;
                break; 
            case '.':
                std::cout << memoria[ptr];
                break;
            case ',':
                std::cin >> memoria[ptr];
                break;
            case '[':
                if (memoria[ptr] == 0) {
                    // Pula direto para o ']' usando o mapa
                    i = info_loops[i]; 
                }
                break;
            case ']':
                if (memoria[ptr] != 0) {
                    // Volta direto para o '[' usando o mapa
                    i = info_loops[i]; 
                }
                break;
        }
    }
    
    std::cout << std::endl; // Pula uma linha no fim da execução
    return 0;
}