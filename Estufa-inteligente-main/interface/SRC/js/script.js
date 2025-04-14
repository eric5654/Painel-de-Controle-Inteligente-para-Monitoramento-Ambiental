// Configuração do gráfico
const dados = {
    labels: [],
    datasets: [{
        label: 'Temperatura (°C)',
        data: [],
        borderColor: 'rgb(255, 99, 132)',
        tension: 0.1
    }, {
        label: 'Umidade do Ar (%)',
        data: [],
        borderColor: 'rgb(54, 162, 235)',
        tension: 0.1
    }, {
        label: 'Umidade do Solo (%)',
        data: [],
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1
    }]
};

const configGrafico = {
    type: 'line',
    data: dados,
    options: {
        responsive: true,
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
};

const grafico = new Chart(
    document.getElementById('grafico'),
    configGrafico
);

// Função para gerar dados simulados e enviar para o banco de dados
function atualizarDadosSimulados() {
    const agora = new Date().toLocaleTimeString();
    const temperatura = Math.floor(Math.random() * 10) + 20;
    const umidadeAr = Math.floor(Math.random() * 20) + 50;
    const umidadeSolo = Math.floor(Math.random() * 30) + 60;

    dados.labels.push(agora);
    dados.datasets[0].data.push(temperatura);
    dados.datasets[1].data.push(umidadeAr);
    dados.datasets[2].data.push(umidadeSolo);
    grafico.update();

    document.getElementById('temperatura').textContent = temperatura;
    document.getElementById('umidadeAr').textContent = umidadeAr;
    document.getElementById('umidadeSolo').textContent = umidadeSolo;
    document.getElementById('dataHora').textContent = agora;

    // Envia os dados simulados para o banco de dados
    fetch('http://127.0.0.1:5008/dados/simulados', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        }
    })
    .then(response => response.json())
    .then(data => {
        console.log("Dados simulados enviados:", data);
    })
    .catch(error => {
        console.error('Erro ao enviar dados simulados:', error);
    });
}

// Função para buscar e atualizar os dados do banco de dados (com fallback para dados simulados)
function atualizarDados() {
    fetch('http://127.0.0.1:5008/dados')
        .then(response => response.json())
        .then(data => {
            if (data && data.length > 0) {
                dados.labels = [];
                dados.datasets[0].data = [];
                dados.datasets[1].data = [];
                dados.datasets[2].data = [];

                data.forEach(item => {
                    dados.labels.push(item.data_hora);
                    dados.datasets[0].data.push(item.temperatura);
                    dados.datasets[1].data.push(item.umidade_ar);
                    dados.datasets[2].data.push(item.umidade_solo);
                });

                grafico.update();

                const ultimoDado = data[data.length - 1];
                document.getElementById('temperatura').textContent = ultimoDado.temperatura;
                document.getElementById('umidadeAr').textContent = ultimoDado.umidade_ar;
                document.getElementById('umidadeSolo').textContent = ultimoDado.umidade_solo;
                document.getElementById('dataHora').textContent = ultimoDado.data_hora;
            } else {
                atualizarDadosSimulados();
            }
        })
        .catch(error => {
            console.error('Erro ao buscar dados:', error);
            atualizarDadosSimulados();
        });
}

// Atualiza os dados a cada 5 segundos
setInterval(atualizarDados, 5000);

// Chama a função para buscar os dados inicialmente
atualizarDados();