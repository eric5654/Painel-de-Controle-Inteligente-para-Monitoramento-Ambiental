from flask import Flask, jsonify
from flask_cors import CORS
import mysql.connector
import random
from datetime import datetime
from apscheduler.schedulers.background import BackgroundScheduler

app = Flask(__name__)
CORS(app)

# Configuração do banco de dados
db_config = {
    "host": "localhost",
    "user": "root",
    "password": "",
    "database": "painel_ambiental",
    "collation": "utf8mb4_general_ci"
}

def get_db_connection():
    """Função para criar uma conexão com o banco de dados."""
    try:
        connection = mysql.connector.connect(**db_config)
        return connection
    except mysql.connector.Error as err:
        print(f"Erro ao conectar ao banco de dados: {err}")
        return None

def gerar_e_armazenar_dados():
    """Função para gerar e armazenar dados simulados no banco de dados."""
    connection = get_db_connection()
    if connection:
        cursor = connection.cursor()

        temperatura = round(random.uniform(20, 30), 2)
        umidade_ar = round(random.uniform(50, 80), 2)
        umidade_solo = round(random.uniform(60, 90), 2)
        data_hora = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        try:
            sql = "INSERT INTO dados_ambientais (temperatura, umidade_ar, umidade_solo, data_hora) VALUES (%s, %s, %s, %s)"
            val = (temperatura, umidade_ar, umidade_solo, data_hora)
            cursor.execute(sql, val)
            connection.commit()
            cursor.close()
            connection.close()
            print("Dados simulados inseridos com sucesso!")
        except mysql.connector.Error as err:
            cursor.close()
            connection.close()
            print(f"Erro ao inserir dados: {err}")
    else:
        print("Não foi possível conectar ao banco de dados")

@app.route('/dados', methods=['GET'])
def get_dados():
    """Rota para buscar todos os dados da tabela dados_ambientais."""
    connection = get_db_connection()
    if connection:
        cursor = connection.cursor(dictionary=True)
        cursor.execute("SELECT * FROM dados_ambientais")
        results = cursor.fetchall()
        cursor.close()
        connection.close()
        return jsonify(results)
    else:
        return jsonify({"error": "Não foi possível conectar ao banco de dados"}), 500

@app.route('/dados/simulados', methods=['POST'])
def inserir_dados_simulados():
    """Rota para inserir dados simulados no banco de dados (usado pelo front-end)."""
    gerar_e_armazenar_dados()
    return jsonify({"message": "Dados simulados inseridos com sucesso!"}), 201

@app.route('/dados/agrupados', methods=['GET'])
def get_dados_agrupados():
    """Rota para buscar os dados agrupados por dia."""
    connection = get_db_connection()
    if connection:
        cursor = connection.cursor(dictionary=True)
        query = """
            SELECT
                DATE(data_hora) AS data,
                AVG(temperatura) AS media_temperatura,
                AVG(umidade_ar) AS media_umidade_ar,
                AVG(umidade_solo) AS media_umidade_solo
            FROM
                dados_ambientais
            GROUP BY
                DATE(data_hora)
            ORDER BY
                DATE(data_hora) DESC;
        """
        cursor.execute(query)
        results = cursor.fetchall()
        cursor.close()
        connection.close()
        return jsonify(results)
    else:
        return jsonify({"error": "Não foi possível conectar ao banco de dados"}), 500

if __name__ == '__main__':
    scheduler = BackgroundScheduler()
    scheduler.add_job(func=gerar_e_armazenar_dados, trigger="interval", seconds=5)  # Agende a tarefa a cada 5 segundos
    scheduler.start()
    app.run(debug=True)