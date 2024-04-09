module summing_op_amp(
    input signed [15:0] in1, // Input voltage 1
    input signed [15:0] in2, // Input voltage 2
    input signed [15:0] in3, // Input voltage 3
    input signed [15:0] in4, // Input voltage 4
    output signed [15:0] out // Output voltage
);

assign out = -(in1 + in2 + in3 + in4); // Sum inputs and apply gain of -1

endmodule

