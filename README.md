# Automated Crypto Trading Bot on Raspberry Pi

## Overview
This project is an automated cryptocurrency trading bot designed to run on a Raspberry Pi, utilizing the **GMX decentralized perpetual exchange** for executing trades. The bot operates with **zero human intervention**, employing a **simple moving average strategy** for trading decisions. The system features comprehensive **logging, error handling, automatic notifications, and statistical reporting**.

## GMX Exchange
GMX is a **decentralized perpetual exchange** that allows traders to open leveraged positions on **BTC, ETH, AVAX, and other top cryptocurrencies** with up to **100x leverage** directly from their wallets. GMX operates on the **Arbitrum and Avalanche networks**, providing deep liquidity and minimal trading costs.

## Features of This Trading Bot
- **Fully Automated Trading**: Executes cryptocurrency trades based on predefined moving average strategies.
- **Perpetual Futures Trading**: Utilizes GMX's decentralized derivatives markets.
- **Robust Error Handling**: Detects and handles missing candlestick data and API failures.
- **Advanced Logging System**: Implements a static logging class to track trade execution and errors.
- **Automatic Email Notifications**: Sends alerts for trade executions and system anomalies.
- **Statistical Performance Tracking**: Generates detailed performance reports for strategy evaluation.
- **Object-Oriented C++ Design**: Built using **modular and extensible class structures**, reinforcing **best OOP practices**.

## Project Structure
```
├── Classes                # Core trading classes (strategy, execution, logging, etc.)
├── Notifications          # Handles email notifications and alerts
├── gmx-api                # GMX API integration for market data and trade execution
├── gmx_python_sdk         # Python SDK for supplementary GMX API interactions
├── nlohmann               # JSON parsing library for configuration and API handling
├── node_modules           # Web3 dependencies for blockchain interactions
├── tests                  # Unit tests for candlestick validation and trade execution
├── wallet                 # Wallet authentication and GMX smart contract interactions
├── .vscode                # VS Code workspace settings
├── .gitignore             # Files to ignore in version control
├── Makefile               # Build automation script
├── configuration_notes.txt # Notes on system configuration and trading parameters
├── main.cpp               # Main entry point for the trading bot
├── package-lock.json      # Dependency lock file
├── package.json           # Dependency configuration file
```

## Technologies Used
- **C++** (Primary trading logic and system architecture)
- **Python** (GMX Python SDK for auxiliary API operations)
- **Web3** (For blockchain interactions, smart contract execution, and wallet management)
- **JSON** (Configuration management and candlestick data handling)
- **GMX API** (For market data, order execution, and liquidity tracking)

## Installation & Setup
### Prerequisites
- Raspberry Pi (ARM64 architecture recommended)
- C++ compiler (g++)
- Python 3 with necessary dependencies
- GMX API credentials
- Email SMTP setup for trade notifications

### Setup Steps
1. Clone the repository:
   ```sh
   git clone <repo-url>
   cd <repo-folder>
   ```
2. Install dependencies:
   ```sh
   make install  # Ensure Makefile supports installation
   ```
3. (Optional) Raspberry Pi uptime optimization
   - View `configuration_notes.txt` for information on optimization
4. Compile and start the bot:
   ```sh
   make
   ./trading_bot  # Or the compiled executable name
   ```

## Lessons Learned
This project reinforced **advanced object-oriented programming (OOP) principles**, requiring a **modular and scalable class hierarchy** to manage trading logic, error handling, and market data efficiently. Implementing **decentralized trading strategies** with **GMX perpetual futures** also required a deep understanding of **blockchain-based liquidity, margin trading, and leverage mechanisms**.

## Future Improvements
- Expand to **multi-strategy trading**, incorporating **momentum, arbitrage, and mean reversion strategies**.
- Enhance the **backtesting engine** for improved performance evaluation.
- Optimize **latency and execution speed** for real-time trading efficiency.
- Implement **on-chain analytics** for deeper trading insights.

---
Developed for high-performance, decentralized trading. 🚀
