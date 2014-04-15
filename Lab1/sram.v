module SRAMDriver(disc, inc, WE, store, OE, valid, readNotWrite, clk, rst);
	output disc /* synthesis LOC="P23" */;
	output inc /* synthesis LOC="P22" */;
	output WE /* synthesis LOC="P21" */;
	output store /* synthesis LOC="P20" */;
	output OE /* synthesis LOC="P19" */;
	output valid /* synthesis LOC="P18" */;
	input readNotWrite /* synthesis LOC="P3" */;
	input clk /* synthesis LOC="P1" */;
	input rst /* synthesis LOC="P2" */;
	reg disc, inc, WE, store, OE, valid;
	integer state;
	
	initial begin
		store = 0;
		valid = 1;
		state = 0;
		WE = 1;
		OE = 1;
		inc = 0;
		disc = 0;
	end
	
	always @(posedge clk) begin
		if (rst == 1) begin
			store = 0;
			valid = 1;
			state = 0;
			WE = 1;
			OE = 1;
			inc = 0;
			disc = 0;
		end
		else begin
			case(state)
			0: 	begin
					if (readNotWrite == 1) begin
						state = 1;
					end
					else begin
						state = 7;
					end
				end
			// Read states
			1:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001011;
					state = 2;
				end
			2: 	begin
					{disc, inc, WE, store, OE, valid} = 6'b011011;
					state = 3;
				end
			3:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001111;
					state = 4;
				end
			4: 	begin
					{disc, inc, WE, store, OE, valid} = 6'b001001;
					state = 5;
				end
			5:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001000;
					state = 6;
				end
			6:	begin
					{disc, inc, WE, store, OE, valid} = 6'b101001;
					state = 0;
				end
			// Write states
			7:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001011;
					state = 8;
				end
			8: 	begin
					{disc, inc, WE, store, OE, valid} = 6'b011011;
					state = 9;
				end
			9:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001111;
					state = 10;
				end
			10:	begin
					{disc, inc, WE, store, OE, valid} = 6'b101011;
					state = 11;
				end
			11:	begin
					{disc, inc, WE, store, OE, valid} = 6'b000011;
					state = 12;
				end
			12:	begin
					{disc, inc, WE, store, OE, valid} = 6'b001010;
					state = 13;   
				end
			13:	begin
					{disc, inc, WE, store, OE, valid} = 6'b101011;
					state = 0;   
				end
			endcase
		end
	end
endmodule
	