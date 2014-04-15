module testBench;
  wire[9:0] outputBits, inputBits;
  wire clk, rst;
  
  tenBitReg myReg(outputBits, inputBits, clk, rst);
  
  myTest tester(inputBits, clk, rst, outputBits);
  
endmodule

module myTest(inputBits, clk, rst, outputBits);
  input[9:0] outputBits; 
  output[9:0] inputBits;
  output clk, rst;
  reg[9:0] inputBits;
  reg clk, rst;

  parameter period = 5;
  parameter stimDelay = 20;
  
  initial begin
    clk = 0;
    rst = 0;
    inputBits[9:0] = { 0 };
  end
    
  always begin
    #period clk = ~clk;
  end
  
  always @(posedge clk) begin
    #period rst = 1;
    #stimDelay inputBits[9:0] = 10'b1111111111;
    #stimDelay inputBits[9:0] = 10'b0000000000;
    #stimDelay inputBits[9:0] = 10'b0101010101;
    #stimDelay inputBits[9:0] = 10'b1010101010;
    #stimDelay inputBits[9:0] = 10'b1111100000;
    #stimDelay inputBits[9:0] = 10'b0000011111;
    #stimDelay;
    $finish;
  end
endmodule
