# Automated Crypto Trading Bot on Raspberry Pi

## Overview
This project is an automated cryptocurrency trading bot designed to run on a Raspberry Pi, utilizing the GMX exchange for executing trades. The bot operates with **zero human intervention**, employing a **simple moving average strategy** for trading decisions. The system features comprehensive **logging, error handling, automatic notifications, and statistical reporting**.

## Features
- **Fully Automated Trading**: Trades cryptocurrencies based on predefined moving average strategies.
- **Robust Error Handling**: Handles missing candlestick data and other edge cases.
- **Logging System**: Implements a static logging class to track trade actions and errors.
- **Automatic Emailing**: Sends notifications for executed trades and system alerts.
- **Statistical Reporting**: Generates statistics to analyze performance over time.
- **Object-Oriented Design**: Built with well-structured **C++ classes**, reinforcing strong OOP principles.

## Project Structure
```
├── Classes                # Core trading classes (strategy, execution, logging, etc.)
├── Notifications          # Handles email notifications and alerts
├── gmx-api                # GMX API integration for fetching market data
├── gmx_python_sdk         # Python SDK for additional GMX API interactions
├── nlohmann               # JSON parsing library for configuration and API handling
├── node_modules           # Required dependencies (likely for Web3 interactions)
├── tests                  # Unit tests for candlestick validation and trade execution
├── wallet                 # Wallet and authentication for GMX trades
├── .vscode                # VS Code workspace settings
├── .gitignore             # Files to ignore in version control
├── Makefile               # Build automation script
├── configuration_notes.txt # Notes on system configuration and design decisions
├── main.cpp               # Main entry point for the trading bot
├── package-lock.json      # Dependency lock file
├── package.json           # Dependency configuration file
```

## Technologies Used
- **C++** (Primary trading logic and system architecture)
- **Python** (GMX Python SDK for supplementary API interactions)
- **Web3** (For blockchain interactions and wallet management)
- **JSON** (Configuration and candlestick data parsing)

## Installation & Setup
### Prerequisites
- Raspberry Pi (or a linux based system)
- C++ compiler (g++)
- Python 3 with necessary dependencies
- GMX API credentials
- Email SMTP setup for notifications

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
3. (Optional) Optimizing uptime on Pi:
   - View `configuration_notes.txt` for optimizing raspberry pi server uptime, these settings got me 100% uptime for 3 months of trading
4. Compile and run the bot:
   ```sh
   make
   ./run
   ```

## Lessons Learned
This project reinforced **strong object-oriented programming (OOP) principles**, requiring a **complex class hierarchy** to manage trading logic, logging, and data handling efficiently. The transition to **C++ for core trading** highlighted the importance of memory management and performance optimizations in high-frequency trading applications.

## Future Improvements
- Implement **additional trading strategies** beyond moving averages.
- Enhance the **backtesting framework** for better strategy validation.
- Improve **error recovery mechanisms** for more resilience in market fluctuations.
- Optimize **Raspberry Pi resource usage** for efficiency.

---
Developed with passion and persistence. 🚀

