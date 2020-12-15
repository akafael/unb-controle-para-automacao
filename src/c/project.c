/**
 *
 */

#include <stdint.h>

enum Estados{
    S_DESLIGADO = 0,
    S_BUFFER_VAZIO,
    S_MOTOR_LIGADO,
    NUM_ESTADOS
};

enum Eventos{
    E_BOTOEIRA_DESLIGA = 0,
    E_BOTOEIRA_PARADA_EMERGENCIA,
    E_SINAL_BUFFER_VAZIO,
    NUM_EVENTOS
}; 

struct Acionamentos
{
    int32_t velocidadeMotor;
    uint8_t luzMotorLigado;
    uint8_t luzMotorSentidoA;
    uint8_t luzMotorSentidoB;
    uint8_t luzBuffer;
};

void desligaSistema(struct Acionamentos *sistema)
{
    sistema->velocidadeMotor = 0;
    sistema->luzMotorLigado = 0;
    sistema->luzMotorSentidoA = 0;
    sistema->luzMotorSentidoB = 0;
    sistema->luzBuffer = 0;
}

void bufferVazio(struct Acionamentos *sistema)
{
    sistema->velocidadeMotor = 0;
    sistema->luzMotorLigado = 0;
    sistema->luzMotorSentidoA = 0;
    sistema->luzMotorSentidoB = 0;
    sistema->luzBuffer = 1;
}

void ligaMotorSentidoA(struct Acionamentos *sistema)
{
    sistema->velocidadeMotor = 100;
    sistema->luzMotorLigado = 1;
    sistema->luzMotorSentidoA = 1;
    sistema->luzMotorSentidoB = 0;
    sistema->luzBuffer = 0;
}

void ligaMotorSentidoB(struct Acionamentos *sistema)
{
    sistema->velocidadeMotor = 0;
    sistema->luzMotorLigado = 1;
    sistema->luzMotorSentidoA = 0;
    sistema->luzMotorSentidoB = 1;
    sistema->luzBuffer = 0;
}

int main()
{
   // Define Matriz de Transição
   int matrizTransicao[NUM_ESTADOS][NUM_EVENTOS];
   matrizTransicao[S_DESLIGADO][E_BOTOEIRA_DESLIGA] = S_DESLIGADO;

   // Associa Estados as Ações Correspondentes
   void (*controleAcionamentos[NUM_ESTADOS])(struct Acionamentos *sistema);
   controleAcionamentos[S_DESLIGADO] = desligaSistema;

   // Teste
   struct Acionamentos sistema;
   int estadoAtual = S_DESLIGADO;
   int eventoNovo = E_BOTOEIRA_DESLIGA;
   int estadoNovo = matrizTransicao[estadoAtual][eventoNovo];
   controleAcionamentos[estadoNovo];
}

