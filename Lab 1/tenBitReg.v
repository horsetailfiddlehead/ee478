module tenBitReg(outputBits, inputBits, clk, rst);
	output[9:0] outputBits /* synthesis LOC="P23P22P21P20P19P18P17P16P15P14" */;
	input[9:0] inputBits /* synthesis LOC="P2P3P4P5P6P7P8P9P10P11" */;
	input clk /* synthesis LOC="P1" */;
	input rst /* synthesis LOC="P13" */;
	
	// reset HIGH because counter is reset high
	
	reg[9:0] outputBits;
	
	initial begin
		outputBits[9:0] = { 0 };
	end
	
	always @(posedge clk) begin
		if (rst == 1) begin
			outputBits[9:0] = { 0 };
		end
		else begin
			outputBits[9:0] = inputBits[9:0];
		end
	end
endmodule
