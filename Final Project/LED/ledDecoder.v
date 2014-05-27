module ledDecoder(outputBits, cardSelect, red, green, clk, rst);
	output[7:0] outputBits /* synthesis LOC="P21P20P19P18P17P16P15P14" */;
	input [1:0] cardSelect /* synthesis LOC="P3P2" */;
	input red /* synthesis LOC="P4" */;
	input green /* synthesis LOC="P5" */;
	input clk /* synthesis LOC="P1" */;
	input rst /* synthesis LOC="P13" */;

	reg[7:0] outputBits;
	
	initial begin
		outputBits[7:0] = { 0 };
	end
	
	/*
	
	Output Bits
	
	Card Select : [  4  ] [  3  ] [  2  ] [  1  ]
	Color Select: [G] [R] [G] [R] [G] [R] [G] [R]
	
	*/
	
	always @ (posedge clk) begin
		if (rst == 1) begin
			outputBits[7:0] = { 0 };	
		end
		// Change card to be selected
		else begin 
			case (cardSelect) 
				0: begin
						outputBits[0] = red;
						outputBits[1] = green;
					end
				1: begin
						outputBits[2] = red;
						outputBits[3] = green;
					end
				2: begin
						outputBits[4] = red;
						outputBits[5] = green;
					end
				3: begin				
						outputBits[6] = red;
						outputBits[7] = green;
					end
				default: begin
						outputBits[7:0] = 0;
					end
			endcase
		end
	end
endmodule
