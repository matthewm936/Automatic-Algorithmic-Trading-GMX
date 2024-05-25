from utils import _set_paths
import argparse

_set_paths()

from scripts.v2.order.order_argument_parser import (
    OrderArgumentParser
)
from scripts.v2.order.create_increase_order import IncreaseOrder

parser = argparse.ArgumentParser(description="Create a GMX increase order.")
parser.add_argument('index_token_symbol', type=str, help="The market you want to trade on (e.g., 'SOL').")
parser.add_argument('is_long', type=lambda x: x.lower() == 'true', help="True for long, False for short.")
parser.add_argument('size', type=float, help="Position size in USD.")
parser.add_argument('leverage', type=float, help="Leverage amount.")

args = parser.parse_args()

parameters = {
    "chain": 'arbitrum',

    # the market you want to trade on
    "index_token_symbol": args.index_token_symbol,

    # token to use as collateral. Start token swaps into collateral token
    # if different
    "collateral_token_symbol": "USDC",

    # the token to start with - WETH not supported yet
    "start_token_symbol": "USDC",

    # True for long, False for short
    "is_long": args.is_long,

    # Position size in in USD
    "size_delta_usd": args.size,

    # if leverage is passed, will calculate number of tokens in
    # start_token_symbol amount
    "leverage": args.leverage,

    # as a decimal ie 0.003 == 0.3%
    "slippage_percent": 0.003
}


order_parameters = OrderArgumentParser(
    is_increase=True
).process_parameters_dictionary(
    parameters
)

order = IncreaseOrder(
    chain=order_parameters['chain'],
    market_key=order_parameters['market_key'],
    collateral_address=order_parameters['start_token_address'],
    index_token_address=order_parameters['index_token_address'],
    is_long=order_parameters['is_long'],
    size_delta=order_parameters['size_delta'],
    initial_collateral_delta_amount=(
        order_parameters['initial_collateral_delta']
    ),
    slippage_percent=order_parameters['slippage_percent'],
    swap_path=order_parameters['swap_path']
)


## from example scripts, I am not using the config dict stuff
# After we are done with operations, set private_key and
# user_wallet_address to None so they are not saved locally
# new_config_dict['private_key'] = None
# new_config_dict['user_wallet_address'] = None

# # set config file
# config_obj.set_config(new_config_dict)