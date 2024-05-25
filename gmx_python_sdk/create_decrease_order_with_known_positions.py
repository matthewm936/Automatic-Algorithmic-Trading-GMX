from utils import _set_paths
import argparse

_set_paths()

from scripts.v2.order.create_decrease_order import DecreaseOrder
from get_positions import (
    get_positions, transform_open_position_to_order_parameters
)


parser = argparse.ArgumentParser(description="Create a GMX increase order.")
parser.add_argument('index_token_symbol', type=str, help="The market you want to trade on (e.g., 'SOL').")
parser.add_argument('is_long', type=lambda x: x.lower() == 'true', help="True for long, False for short.")

args = parser.parse_args()

chain = "arbitrum"
market_symbol = args.index_token_symbol
out_token = "USDC"
is_long = args.is_long
slippage_percent = 0.003
amount_of_position_to_close = 1
amount_of_collateral_to_remove = 1

# gets all open positions as a dictionary, which the keys as each position
positions = get_positions(chain)

order_parameters = transform_open_position_to_order_parameters(chain,
                                                               positions,
                                                               market_symbol,
                                                               is_long,
                                                               slippage_percent,
                                                               out_token,
                                                               amount_of_position_to_close,
                                                               amount_of_collateral_to_remove)


order = DecreaseOrder(
    chain=order_parameters['chain'],
    market_key=order_parameters['market_key'],
    collateral_address=order_parameters['collateral_address'],
    index_token_address=order_parameters['index_token_address'],
    is_long=order_parameters['is_long'],
    size_delta=order_parameters['size_delta'],
    initial_collateral_delta_amount=(
        order_parameters['initial_collateral_delta']
    ),
    slippage_percent=order_parameters['slippage_percent'],
    swap_path=order_parameters['swap_path']
)
