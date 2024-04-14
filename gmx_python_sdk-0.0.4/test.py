from estimate_swap_output import EstimateSwapOutput

chain = "arbitrum"
in_token_symbol = "SOL"
out_token_symbol = "USDC"
token_amount = 2
in_token_address = None
out_token_address = None
token_amount_expanded = None

output = EstimateSwapOutput(chain=chain).get_swap_output(
    in_token_symbol=in_token_symbol,
    out_token_symbol=out_token_symbol,
    token_amount=token_amount,
    in_token_address=in_token_address,
    out_token_address=out_token_address,
    token_amount_expanded=token_amount_expanded
)