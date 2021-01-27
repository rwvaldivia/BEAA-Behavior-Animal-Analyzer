# BEAA - Behavior Animal Analyzer
Open source software written in Java and C
<h1 align="center">
<img src="https://github.com/rwvaldivia/BEAA-Behavior-Animal-Analyzer/blob/master/Processamento.png">
</h1>

---

# Resumo

Esta é uma ferramenta no formato de software livre, baseado em Visão Computacional com o auxílio da biblioteca openCV para Processamento de Imagens de ratos de laboratório da espécie Sprague Dawlay (brancos albinos) para análise de um comportamento específico. 

O objetivo é, através do processamento computacional, verificar a periodicidade de convulsões epiléticas que ocorrem durante um período de tempo ao longo do tratamento da espécie com um nível ótimo de precisão nessa avaliação que atualmente vem sendo realizado de forma manual por alunos de laboratório do Departamento de Neurofisiologia da Escola Paulista de Medicina - EPM.

# Justificativa e definição do problema
Sistemas que deem suporte à Visão Computacional (VC) são hoje uma realidade em diversas áreas do conhecimento. A VC é uma área relacionada à computação gráfica e que ligada ao reconhecimento de face, rastreamento de objetos e detecção de movimentos utiliza o processamento de imagem e processamento de vídeo como recurso para análise de ambiente se tornando ótimo para esse trabalho.

Atualmente no laboratório de Neurofisiologia do Departamento de Fisiologia da Escola Paulista de Medicina é realizada pesquisas em ratos da raça Sprague Dawlay com a manipulação de células tronco para o tratamento de epilepsias através de estudos. Os modelos experimentais (ratos) são primeiramente manipulados com a droga Pilocarpina. Essa droga ativa as vias colinérgicas e induz o rato a epilepsia, potencializando a ação de neuros-transmissores deixando-os excitados. Após esse primeiro momento são observadas as frequências de crises convulsivas nos modelos.

Os animais são filmados em caixas individuais diariamente com a periodicidade entre dois a três meses, gerando muitas horas de filmagem que depois são assistidos manualmente por alunos para a identificação e quantificação das crises de cada animal.

Cada vídeo grava 12 animais simultaneamente durante o período de observação de 8 horas. Dessa forma são realizadas anotações sobre datas e horários das convulsões. A ferramenta deverá ser capaz de realizar tais apontamentos de forma on-line de modo a mitigar o trabalho feito por observadores humanos e apresentar os resultados para posterior análise e validação pelos especialistas.

Assim a proposta de pesquisa é realizar o monitoramento de ratos de laboratório do Departamento de Neurofisiologia da Escola Paulista de Medicina e quantificar os resultados diários de convulsões epiléticas ocorridas através de um sistema baseado em Visão Computacional e Processamento de imagens. Será avaliada a viabilidade de implementação e execução do projeto e determinadas as métricas para avaliação da solução proposta.


---

<h1 align="center">
<img src="https://github.com/rwvaldivia/BEAA-Behavior-Animal-Analyzer/blob/master/Behavior.png">
</h1>

