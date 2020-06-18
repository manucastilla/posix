# POSIX
O objetivo deste trabalho foi trabalhar e focar em conceitos de sistemas operacionais, mais especificamente com o padrão POSIX. Assim, neste lab foi feito um framework de testes usando as chamadas de sistemas, aprimorando então o programa.


**Estrutura do projeto**
- ```example.c```: neste arquivo contêm todos os testes que o programa roda. Nela contêm tais testes:
    1. Cause erro e termine com falha de segmentação
    2. Cause erro e termine com divisão por zero 
    3. Fique em loop infinito
    4. Faça muito trabalho, mas eventualmente acabe (sem usar sleep ) 
    5. Com asserts que falham e passem no mesmo teste 
    6. Com testes que façam muitos prints 
    7. Com testes que sejam rápidos
    8. Com testes que sejam lentos (pode usar sleep para simular).
    
- ```macros.h```: neste arquivo contêm os *macros* usada dentro das funções de teste para dar prints e verificar se o teste passa ou não

- ```runner.h```: este arquivo chama os testes (tanto um, quanto todos os testes) e mostra seus resultados. 

**Funções do arquivo principal: ```runner.h```**

- ```sigint_handler()```:  se o usuário apertar Ctrl + C, verifica se ele realmetne deseja acabar o programa. 

- ```main()```:  chama os testes (tanto um, quanto todos os testes) e mostra seus resultados. 
   
    
**Passo a passo no main()**

 - na primeira parte do main é definido o *clock*, a *quantidade* de testes a ser rodado, como também a variável *wt* (para usar posteriamente no wait) e o *pass_count* (para contabilizar quantos testes de fato passaram).
 - antes de rodas os testes é inicializado também o handler possibilitando a função de Ctrl + C
 - temos duas partes principais dentro do main:
    - caso em executar um teste específico
    - caso em executar todos os testes de maneira isolada e parelelo
    
  Ambos os casos a lógica é parecida, apenas com algumas modificações. Assim, no caso mais geral (o caso de rodar todos os testes) temos então três partes:
  - executar os testes
  - verificar se os testes rodaram com sucesso ou houve algum problema
  - organizar seus prints e seus resultados no terminal
 
  Todas essas etapas englobam três itens principais: executar os testes, relatório de erros dos testes que falharam e sáida padrão e erros de cada teste.

- **executar os testes**: 
   - Primeiramente é necessário criar um for. A cada iteração, é criado um arquivo para cada teste (para organizar sua saída) como também feito um fork (possibilitando de que os testes rodem paralelamente e isolados). 
   - Ao entrar no filho (no teste específico) é iniciado o tempo e o alarme de 2 segundos caso o teste demore muito. Além disso, é feito uma cópia (```dup2```) de tudo que for printado para o arquivo criado.
   - Com isso, se o teste passar, é escrito no relatório (em cada arquivo) que passou e quanto tempo demorou para rodar. Caso rode o teste, porém não está correto, tal teste é escrito que ocorreu algum erro. 
   - A cada teste que rodar, falhando ou não, são colocados em um vetor de ```pid_t```, possibilitando sua identificação. 
   
- **Análise de cada teste**:
   - Para que seja possível a análise de cada teste, utilizamos *sinais*. Assim, utilizados ```waitpid``` na qual espera o término de execução de um específico teste. Como o ```waitpid``` está em um while, este rodara todos os testes. 
   - temos então duas possíveis resultados quando o filho acaba de executar: quando ocorre tudo normal (*WIFEXITED*) e de fato deu certo (*WEXITSTATUS*), o ```pass_count``` ira contabilizar como um teste passado. Caso o teste não aconteceu o esperado temos então duas opções para isso: ou deu um erro ou seu tempo acabou. 
   -  todas essas informações são escritas em cada arquivo específico.
   
- **Resultado no terminal**:
   - Neste ponto do programa temos todos os testes rodados e analisados (cada arquivo contêm se deu errado, se deu certo ou se o teste falhou). Agora é preciso enviar estes arquivos para o terminal novamente. Para isso, no início do código criamos uma variável *saved_stdout* e que nela fosse guardado ```dup(1)``` . Com isso, é possível fazer com que tudo que está escrito nos arquivos volte para o terminal.
   - Assim, é feito um for que passe por todos os arquivos, lendo estes e escrevendo no terminal. 
