-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Tempo de geração: 08/04/2025 às 00:25
-- Versão do servidor: 11.3.0-MariaDB
-- Versão do PHP: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `painel_ambiental`
--

-- --------------------------------------------------------

--
-- Estrutura para tabela `dados_ambientais`
--

CREATE TABLE `dados_ambientais` (
  `id` int(11) NOT NULL,
  `temperatura` decimal(5,2) DEFAULT NULL,
  `umidade_ar` decimal(5,2) DEFAULT NULL,
  `umidade_solo` decimal(5,2) DEFAULT NULL,
  `data_hora` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Despejando dados para a tabela `dados_ambientais`
--

INSERT INTO `dados_ambientais` (`id`, `temperatura`, `umidade_ar`, `umidade_solo`, `data_hora`) VALUES
(516, 28.05, 74.95, 77.32, '2025-04-07 19:02:42'),
(517, 27.75, 76.23, 60.03, '2025-04-07 19:02:42'),
(518, 24.96, 67.24, 79.52, '2025-04-07 19:02:47'),
(519, 26.75, 62.47, 73.16, '2025-04-07 19:02:47'),
(520, 26.10, 67.00, 88.64, '2025-04-07 19:02:52'),
(521, 22.67, 64.46, 75.87, '2025-04-07 19:02:52'),
(522, 25.47, 72.28, 79.64, '2025-04-07 19:02:57'),
(523, 26.18, 72.42, 67.11, '2025-04-07 19:02:57'),
(524, 28.85, 63.55, 81.88, '2025-04-07 19:03:02'),
(525, 29.82, 73.98, 81.38, '2025-04-07 19:03:02'),
(526, 26.88, 54.84, 81.03, '2025-04-07 19:03:07'),
(527, 29.78, 51.00, 62.20, '2025-04-07 19:03:07'),
(528, 22.03, 53.20, 84.40, '2025-04-07 19:03:12'),
(529, 22.59, 65.50, 64.14, '2025-04-07 19:03:12');

--
-- Índices para tabelas despejadas
--

--
-- Índices de tabela `dados_ambientais`
--
ALTER TABLE `dados_ambientais`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT para tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `dados_ambientais`
--
ALTER TABLE `dados_ambientais`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=530;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
