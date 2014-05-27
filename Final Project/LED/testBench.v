module testBench;
  wire [7:0] outputBits /* synthesis LOC="P23P22P21P20P19P18P17" */;
  wire [1:0] cardSelect /* synthesis LOC="P2P3" */;
	wire red /* synthesis LOC="P4" */;
	wire green /* synthesis LOC="P5" */;
  ledDecoder myDecoder(outputBits, cardSelect, red, green);
  
  myTest tester(cardSelect, red, green, outputBits);
  
endmodule

module myTest(cardSelect, red, green, outputBits);
  input [7:0] outputBits /* synthesis LOC="P23P22P21P20P19P18P17" */;
	output [1:0] cardSelect /* synthesis LOC="P2P3" */;
	output red /* synthesis LOC="P4" */;
	output green /* synthesis LOC="P5" */;
  
  reg [1:0] cardSelect;
  reg red;
  reg green;
  

  parameter period = 5;
  parameter stimDelay = 20;
  
  initial begin
    #stimDelay cardSelect[1:0] = 2'b00;
                          red = 0;
                        green = 0;
    #stimDelay cardSelect[1:0] = 2'b01;
                          red = 1;
                        green = 0;
    #stimDelay cardSelect[1:0] = 2'b10;
                          red = 0;
                        green = 1;
    #stimDelay cardSelect[1:0] = 2'b11;
                          red = 1;
                        green = 1;
    #stimDelay
    $finish;
  end
endmodule
