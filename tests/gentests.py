Equiv = {
    0b000011001: 0b000001000,
    0b001111001: 0b001101000,
    0b010011001: 0b010001000,
    0b011011011: 0b011011001,
    0b011111011: 0b011111001,
    0b000011110: 0b000011100,
    0b011011110: 0b000011100,
    0b011111110: 0b011111100,
    0b000011111: 0b000011000,
    0b011011111: 0b011011000,
    0b011111111: 0b011111000,
    0b101011010: 0b101010000,
    0b100111101: 0b000111101,
    0b101111101: 0b001111101,
    0b110111101: 0b010111101,
    0b111111101: 0b011111101,
    0b000001001: 0b000001000,
    0b000001010: 0b000001000,
    0b000001011: 0b000001000,
    0b000001100: 0b000001000,
    0b000001101: 0b000001000,
    0b000001110: 0b000001000,
    0b000001111: 0b000001000,
    0b010001001: 0b010001000,
    0b010001010: 0b010001000,
    0b010001011: 0b010001000,
    0b010001100: 0b010001000,
    0b010001101: 0b010001000,
    0b010001110: 0b010001000,
    0b010001111: 0b010001000,
    0b010101001: 0b010101000,
    0b010101010: 0b010101000,
    0b010101011: 0b010101000,
    0b010101100: 0b010101000,
    0b010101101: 0b010101000,
    0b010101110: 0b010101000,
    0b010101111: 0b010101000,
}

Modes = {
    0b00000: ("РОН0", "РОН0", "РР"),
    0b00001: ("РОН1", "РОН1", "РР"),
    0b00010: ("РОН2", "РОН2", "РР"),
    0b00011: ("РОН3", "РОН3", "РР"),
    0b00100: ("РОН4", "РОН4", "РР"),
    0b00101: ("РОН5", "РОН5", "РР"),
    0b00110: ("РОН6", "РОН6", "РР"),
    0b00111: ("РОН7", "РОН7", "РР"),
    0b01000: ("РР", "РОН0", "РР"),
    0b01001: ("РР", "РОН1", "РР"),
    0b01010: ("РР", "РОН2", "РР"),
    0b01011: ("РР", "РОН3", "РР"),
    0b01100: ("РР", "РОН4", "РР"),
    0b01101: ("РР", "РОН5", "РР"),
    0b01110: ("РР", "РОН6", "РР"),
    0b01111: ("РР", "РОН7", "РР"),
    0b11000: ("ШИНвых", "ШИНвх", "РР"),
    0b11001: ("РР", "ШИНвх", "РР"),
    0b11011: ("РР", "ШИНвх", "РРР"),
    0b11100: ("РРР", "ШИНвх", "РР"),
    0b11110: ("РРР", "ШИНвх", "РРР"),
    0b11111: ("ШИНвых", "ШИНвх", "РРР"),
}

ModeNames = {
    0b00000: "reg0",
    0b00001: "reg1",
    0b00010: "reg2",
    0b00011: "reg3",
    0b00100: "reg4",
    0b00101: "reg5",
    0b00110: "reg6",
    0b00111: "reg7",
    0b01000: "wr-reg0",
    0b01001: "wr-reg1",
    0b01010: "wr-reg2",
    0b01011: "wr-reg3",
    0b01100: "wr-reg4",
    0b01101: "wr-reg5",
    0b01110: "wr-reg6",
    0b01111: "wr-reg7",
    0b11000: "di-wr-to-do",
    0b11001: "di-wr-to-wr",
    0b11011: "di-xwr-to-wr", 
    0b11100: "di-wr-to-xwr",
    0b11110: "di-xwr-to-xwr",
    0b11111: "di-xwr-to-do",
}

Formats = {
    0b0000: ("{V} := !П", None, None),
    0b0001: ("{V} := {B} - {A} - 1 + П", "{V} := {B} - {A} - 1", "{V} := {B} - {A}"),
    0b0010: ("{V} := {A} - {B} - 1 + П", "{V} := {A} - {B} - 1", "{V} := {A} - {B}"),
    0b0011: ("{V} := {A} + {B} + П", "{V} := {A} + {B}", "{V} := {A} + {B} + 1"),
    0b0100: ("{V} := {B} + П", "{V} := {B}", "{V} := {B} + 1"),
    0b0101: ("{V} := !{B} + П", "{V} := !{B}", "{V} := !{B} + 1"),
    0b0110: ("{V} := {A} + П", "{V} := {A}", "{V} := {A} + 1"),
    0b0111: ("{V} := !{A} + П", "{V} := !{A}", "{V} := !{A} + 1"),
    0b1000: ("{V} := {A} and {B}", None, None),
    0b1001: ("{V} := {A} xor {B}", None, None),
    0b1010: ("{V} := !({A} xor {B})", None, None),
    0b1011: ("{V} := !{A} and {B}", None, None),
    0b1100: ("{V} := {A} and !{B}", None, None),
    0b1101: ("{V} := {A} or !{B}", None, None),
    0b1110: ("{V} := !{A} or {B}", None, None),
    0b1111: ("{V} := {A} or {B}", None, None)
}

Sum = {
    0b001110000: ("РРР := РОН0 + РР + П", "РРР := РОН0 + РР", "РРР := РОН0 + РР + 1"),
    0b001110001: ("РРР := РОН1 + РР + П", "РРР := РОН1 + РР", "РРР := РОН1 + РР + 1"),
    0b001110010: ("РРР := РОН2 + РР + П", "РРР := РОН2 + РР", "РРР := РОН2 + РР + 1"),
    0b001110011: ("РРР := РОН3 + РР + П", "РРР := РОН3 + РР", "РРР := РОН3 + РР + 1"),
    0b001110100: ("РРР := РОН4 + РР + П", "РРР := РОН4 + РР", "РРР := РОН4 + РР + 1"),
    0b001110101: ("РРР := РОН5 + РР + П", "РРР := РОН5 + РР", "РРР := РОН5 + РР + 1"),
    0b001110110: ("РРР := РОН6 + РР + П", "РРР := РОН6 + РР", "РРР := РОН6 + РР + 1"),
    0b001110111: ("РРР := РОН7 + РР + П", "РРР := РОН7 + РР", "РРР := РОН7 + РР + 1"),
    0b010010000: ("РР := РОН0 + ШИНвх + П", "РР := РОН0 + ШИНвх", "РР := РОН0 + ШИНвх + 1"),
    0b010010001: ("РР := РОН1 + ШИНвх + П", "РР := РОН1 + ШИНвх", "РР := РОН1 + ШИНвх + 1"),
    0b010010010: ("РР := РОН2 + ШИНвх + П", "РР := РОН2 + ШИНвх", "РР := РОН2 + ШИНвх + 1"),
    0b010010011: ("РР := РОН3 + ШИНвх + П", "РР := РОН3 + ШИНвх", "РР := РОН3 + ШИНвх + 1"),
    0b010010100: ("РР := РОН4 + ШИНвх + П", "РР := РОН4 + ШИНвх", "РР := РОН4 + ШИНвх + 1"),
    0b010010101: ("РР := РОН5 + ШИНвх + П", "РР := РОН5 + ШИНвх", "РР := РОН5 + ШИНвх + 1"),
    0b010010110: ("РР := РОН6 + ШИНвх + П", "РР := РОН6 + ШИНвх", "РР := РОН6 + ШИНвх + 1"),
    0b010010111: ("РР := РОН7 + ШИНвх + П", "РР := РОН7 + ШИНвх", "РР := РОН7 + ШИНвх + 1"),
    0b010110000: ("РРР := РОН0 + ШИНвх + П", "РРР := РОН0 + ШИНвх", "РРР := РОН0 + ШИНвх + 1"),
    0b010110001: ("РРР := РОН1 + ШИНвх + П", "РРР := РОН1 + ШИНвх", "РРР := РОН1 + ШИНвх + 1"),
    0b010110010: ("РРР := РОН2 + ШИНвх + П", "РРР := РОН2 + ШИНвх", "РРР := РОН2 + ШИНвх + 1"),
    0b010110011: ("РРР := РОН3 + ШИНвх + П", "РРР := РОН3 + ШИНвх", "РРР := РОН3 + ШИНвх + 1"),
    0b010110100: ("РРР := РОН4 + ШИНвх + П", "РРР := РОН4 + ШИНвх", "РРР := РОН4 + ШИНвх + 1"),
    0b010110101: ("РРР := РОН5 + ШИНвх + П", "РРР := РОН5 + ШИНвх", "РРР := РОН5 + ШИНвх + 1"),
    0b010110110: ("РРР := РОН6 + ШИНвх + П", "РРР := РОН6 + ШИНвх", "РРР := РОН6 + ШИНвх + 1"),
    0b010110111: ("РРР := РОН7 + ШИНвх + П", "РРР := РОН7 + ШИНвх", "РРР := РОН7 + ШИНвх + 1"),
    0b011110000: ("РОН0 := РОН0 + ШИНвх + П", "РОН0 := РОН0 + ШИНвх", "РОН0 := РОН0 + ШИНвх + 1"),
    0b011110001: ("РОН1 := РОН1 + ШИНвх + П", "РОН1 := РОН1 + ШИНвх", "РОН1 := РОН1 + ШИНвх + 1"),
    0b011110010: ("РОН2 := РОН2 + ШИНвх + П", "РОН2 := РОН2 + ШИНвх", "РОН2 := РОН2 + ШИНвх + 1"),
    0b011110011: ("РОН3 := РОН3 + ШИНвх + П", "РОН3 := РОН3 + ШИНвх", "РОН3 := РОН3 + ШИНвх + 1"),
    0b011110100: ("РОН4 := РОН4 + ШИНвх + П", "РОН4 := РОН4 + ШИНвх", "РОН4 := РОН4 + ШИНвх + 1"),
    0b011110101: ("РОН5 := РОН5 + ШИНвх + П", "РОН5 := РОН5 + ШИНвх", "РОН5 := РОН5 + ШИНвх + 1"),
    0b011110110: ("РОН6 := РОН6 + ШИНвх + П", "РОН6 := РОН6 + ШИНвх", "РОН6 := РОН6 + ШИНвх + 1"),
    0b011110111: ("РОН7 := РОН7 + ШИНвх + П", "РОН7 := РОН7 + ШИНвх", "РОН7 := РОН7 + ШИНвх + 1"),
    0b110010000: ("РР := РОН0 + РРР + П", "РР := РОН0 + РРР", "РР := РОН0 + РРР + 1"),
    0b110010001: ("РР := РОН1 + РРР + П", "РР := РОН1 + РРР", "РР := РОН1 + РРР + 1"),
    0b110010010: ("РР := РОН2 + РРР + П", "РР := РОН2 + РРР", "РР := РОН2 + РРР + 1"),
    0b110010011: ("РР := РОН3 + РРР + П", "РР := РОН3 + РРР", "РР := РОН3 + РРР + 1"),
    0b110010100: ("РР := РОН4 + РРР + П", "РР := РОН4 + РРР", "РР := РОН4 + РРР + 1"),
    0b110010101: ("РР := РОН5 + РРР + П", "РР := РОН5 + РРР", "РР := РОН5 + РРР + 1"),
    0b110010110: ("РР := РОН6 + РРР + П", "РР := РОН6 + РРР", "РР := РОН6 + РРР + 1"),
    0b110010111: ("РР := РОН7 + РРР + П", "РР := РОН7 + РРР", "РР := РОН7 + РРР + 1"),
    0b110110000: ("РРР := РОН0 + РРР + П", "РРР := РОН0 + РРР", "РРР := РОН0 + РРР + 1"),
    0b110110001: ("РРР := РОН1 + РРР + П", "РРР := РОН1 + РРР", "РРР := РОН1 + РРР + 1"),
    0b110110010: ("РРР := РОН2 + РРР + П", "РРР := РОН2 + РРР", "РРР := РОН2 + РРР + 1"),
    0b110110011: ("РРР := РОН3 + РРР + П", "РРР := РОН3 + РРР", "РРР := РОН3 + РРР + 1"),
    0b110110100: ("РРР := РОН4 + РРР + П", "РРР := РОН4 + РРР", "РРР := РОН4 + РРР + 1"),
    0b110110101: ("РРР := РОН5 + РРР + П", "РРР := РОН5 + РРР", "РРР := РОН5 + РРР + 1"),
    0b110110110: ("РРР := РОН6 + РРР + П", "РРР := РОН6 + РРР", "РРР := РОН6 + РРР + 1"),
    0b110110111: ("РРР := РОН7 + РРР + П", "РРР := РОН7 + РРР", "РРР := РОН7 + РРР + 1"),
    0b111010000: ("РОН0 := РРР + П", "РОН0 := РРР", "РОН0 := РРР + 1"),
    0b111010001: ("РОН1 := РРР + П", "РОН1 := РРР", "РОН1 := РРР + 1"),
    0b111010010: ("РОН2 := РРР + П", "РОН2 := РРР", "РОН2 := РРР + 1"),
    0b111010011: ("РОН3 := РРР + П", "РОН3 := РРР", "РОН3 := РРР + 1"),
    0b111010100: ("РОН4 := РРР + П", "РОН4 := РРР", "РОН4 := РРР + 1"),
    0b111010101: ("РОН5 := РРР + П", "РОН5 := РРР", "РОН5 := РРР + 1"),
    0b111010110: ("РОН6 := РРР + П", "РОН6 := РРР", "РОН6 := РРР + 1"),
    0b111010111: ("РОН7 := РРР + П", "РОН7 := РРР", "РОН7 := РРР + 1"),
    0b001111010: ("РРР := РР + ШИНвх + П", "РРР := РР + ШИНвх", "РРР := РР + ШИНвх + 1"),
    0b011111010: ("ШИНвых := РР + ШИНвх + П", "ШИНвых := РР + ШИНвх", "ШИНвых := РР + ШИНвх + 1"),
    0b110011010: ("РР := РРР + ШИНвх + П", "РР := РРР + ШИНвх", "РР := РРР + ШИНвх + 1"),
    0b110111010: ("РРР := РРР + ШИНвх + П", "РРР := РРР + ШИНвх", "РРР := РРР + ШИНвх + 1"),
    0b111011010: ("ШИНвых := РРР + П", "ШИНвых := РРР", "ШИНвых := РРР + 1"),
    0b111110000: ("РОН0 := ШИНвх", None, None),
    0b111110001: ("РОН1 := ШИНвх", None, None),
    0b111110010: ("РОН2 := ШИНвх", None, None),
    0b111110011: ("РОН3 := ШИНвх", None, None),
    0b111110100: ("РОН4 := ШИНвх", None, None),
    0b111110101: ("РОН5 := ШИНвх", None, None),
    0b111110110: ("РОН6 := ШИНвх", None, None),
    0b111110111: ("РОН7 := ШИНвх", None, None),
    0b000010000: ("ШИНвых := РОН0", None, None),
    0b000010001: ("ШИНвых := РОН1", None, None),
    0b000010010: ("ШИНвых := РОН2", None, None),
    0b000010011: ("ШИНвых := РОН3", None, None),
    0b000010100: ("ШИНвых := РОН4", None, None),
    0b000010101: ("ШИНвых := РОН5", None, None),
    0b000010110: ("ШИНвых := РОН6", None, None),
    0b000010111: ("ШИНвых := РОН7", None, None),
    0b000110000: ("РРР := РОН0", None, None),
    0b000110001: ("РРР := РОН1", None, None),
    0b000110010: ("РРР := РОН2", None, None),
    0b000110011: ("РРР := РОН3", None, None),
    0b000110100: ("РРР := РОН4", None, None),
    0b000110101: ("РРР := РОН5", None, None),
    0b000110110: ("РРР := РОН6", None, None),
    0b000110111: ("РРР := РОН7", None, None),
    0b011010000: ("РР := ШИНвх", None, None),
    0b000111010: ("РРР := ШИНвх", None, None),
    0b111111010: ("ШИНвых := ШИНвх", None, None)
}

Shift = {
    0b100011010: (1, "СЦЛ", "РР - ШИНвх - 1 + П", "РР - ШИНвх - 1", "РР - ШИНвх"),
    0b100111010: (1, "СЦЛ", "РР + ШИНвх + П", "РР + ШИНвх", "РР + ШИНвх + 1"),
    0b100010000: (1, "СЦЛ", "РР - РОН0 - 1 + П", "РР - РОН0 - 1", "РР - РОН0"),
    0b100010001: (1, "СЦЛ", "РР - РОН1 - 1 + П", "РР - РОН1 - 1", "РР - РОН1"),
    0b100010010: (1, "СЦЛ", "РР - РОН2 - 1 + П", "РР - РОН2 - 1", "РР - РОН2"),
    0b100010011: (1, "СЦЛ", "РР - РОН3 - 1 + П", "РР - РОН3 - 1", "РР - РОН3"),
    0b100010100: (1, "СЦЛ", "РР - РОН4 - 1 + П", "РР - РОН4 - 1", "РР - РОН4"),
    0b100010101: (1, "СЦЛ", "РР - РОН5 - 1 + П", "РР - РОН5 - 1", "РР - РОН5"),
    0b100010110: (1, "СЦЛ", "РР - РОН6 - 1 + П", "РР - РОН6 - 1", "РР - РОН6"),
    0b100010111: (1, "СЦЛ", "РР - РОН7 - 1 + П", "РР - РОН7 - 1", "РР - РОН7"),
    0b100110000: (1, "СЦЛ", "РР + РОН0 + П", "РР + РОН0", "РР + РОН0 + 1"),
    0b100110001: (1, "СЦЛ", "РР + РОН1 + П", "РР + РОН1", "РР + РОН1 + 1"),
    0b100110010: (1, "СЦЛ", "РР + РОН2 + П", "РР + РОН2", "РР + РОН2 + 1"),
    0b100110011: (1, "СЦЛ", "РР + РОН3 + П", "РР + РОН3", "РР + РОН3 + 1"),
    0b100110100: (1, "СЦЛ", "РР + РОН4 + П", "РР + РОН4", "РР + РОН4 + 1"),
    0b100110101: (1, "СЦЛ", "РР + РОН5 + П", "РР + РОН5", "РР + РОН5 + 1"),
    0b100110110: (1, "СЦЛ", "РР + РОН6 + П", "РР + РОН6", "РР + РОН6 + 1"),
    0b100110111: (1, "СЦЛ", "РР + РОН7 + П", "РР + РОН7", "РР + РОН7 + 1"),
    0b101010000: (1, "САП", "РР + П", "РР", "РР + 1"),
    0b001011010: (1, "САП", "РР - ШИНвх - 1 + П", "РР - ШИНвх - 1", "РР - ШИНвх"),
    0b101111010: (1, "САП", "РР + ШИНвх + П", "РР + ШИНвх", "РР + ШИНвх + 1"),
    0b001010000: (1, "САП", "РР - РОН0 - 1 + П", "РР - РОН0 - 1", "РР - РОН0"),
    0b001010001: (1, "САП", "РР - РОН1 - 1 + П", "РР - РОН1 - 1", "РР - РОН1"),
    0b001010010: (1, "САП", "РР - РОН2 - 1 + П", "РР - РОН2 - 1", "РР - РОН2"),
    0b001010011: (1, "САП", "РР - РОН3 - 1 + П", "РР - РОН3 - 1", "РР - РОН3"),
    0b001010100: (1, "САП", "РР - РОН4 - 1 + П", "РР - РОН4 - 1", "РР - РОН4"),
    0b001010101: (1, "САП", "РР - РОН5 - 1 + П", "РР - РОН5 - 1", "РР - РОН5"),
    0b001010110: (1, "САП", "РР - РОН6 - 1 + П", "РР - РОН6 - 1", "РР - РОН6"),
    0b001010111: (1, "САП", "РР - РОН7 - 1 + П", "РР - РОН7 - 1", "РР - РОН7"),
    0b101110000: (1, "САП", "РР + РОН0 + П", "РР + РОН0", "РР + РОН0 + 1"),
    0b101110001: (1, "САП", "РР + РОН1 + П", "РР + РОН1", "РР + РОН1 + 1"),
    0b101110010: (1, "САП", "РР + РОН2 + П", "РР + РОН2", "РР + РОН2 + 1"),
    0b101110011: (1, "САП", "РР + РОН3 + П", "РР + РОН3", "РР + РОН3 + 1"),
    0b101110100: (1, "САП", "РР + РОН4 + П", "РР + РОН4", "РР + РОН4 + 1"),
    0b101110101: (1, "САП", "РР + РОН5 + П", "РР + РОН5", "РР + РОН5 + 1"),
    0b101110110: (1, "САП", "РР + РОН6 + П", "РР + РОН6", "РР + РОН6 + 1"),
    0b101110111: (1, "САП", "РР + РОН7 + П", "РР + РОН7", "РР + РОН7 + 1"),
    0b000011101: (0, "САП", "РР + П", "РР", "РР + 1"),
    0b000111101: (0, "СЦП", "РР + П", "РР", "РР + 1"),
    0b001011101: (0, "САЛ", "РР + П", "РР", "РР + 1"),
    0b001111101: (0, "СЦЛ", "РР + П", "РР", "РР + 1"),
    0b100011101: (0, "СЛП", "РР + П", "РР", "РР + 1"),
    0b101011101: (0, "СЛЛ", "РР + П", "РР", "РР + 1"),
    0b010111101: (1, "СЦП", "РР + П", "РР", "РР + 1"),
    0b011011101: (1, "САЛ", "РР + П", "РР", "РР + 1"),
    0b011111101: (1, "СЦЛ", "РР + П", "РР", "РР + 1"),
    0b110011101: (1, "СЛП", "РР + П", "РР", "РР + 1"),
    0b111011101: (1, "СЛЛ", "РР + П", "РР", "РР + 1"),
}

NOP = 154
N = 1024

X584 = [NOP] * N
Source = ""

TestIndex = 1

def clear():
    global X584, Pos
    for i in range(N):
        X584[i] = NOP
    Pos = 0

def export(name):
    global TestIndex, Source, X584
    print(TestIndex)

    x584_name = "expected-{:04d}".format(TestIndex)
    if not (name is None) or len(name) > 0:
        x584_name += "-" + name
    x584_name += ".x584"

    asm_name = "good-{:04d}".format(TestIndex)
    if not (name is None) or len(name) > 0:
        asm_name += "-" + name
    asm_name += ".asm"

    with open(x584_name, "wb") as f:
        f.write(b'X584')
        for x in X584:
            f.write(x.to_bytes(2, "little"))
            f.write(b'\0')
        f.write(b'V2.0')
        for i in range(N):
            f.write(b'\0\0')
        clear()

    with open(asm_name, "wt", encoding="utf-8") as f:
        f.write(Source)
        Source = ""

    TestIndex += 1

Pos = 0

def write_alu(mode, op, carry):
    global X584, Pos, Source
    if carry == 2:
        if Formats[op][1] is None:
            return
    elif carry == 3:
        if Formats[op][2] is None:
            return
    Opcode = mode | (op << 5)
    if Opcode in Equiv:
        Opcode = Equiv[Opcode]
    X584[Pos] = Opcode
    V, A, B = Modes[mode]
    if carry == 0 or carry == 1:
        if Formats[op][0].find("П") != -1:
            X584[Pos] |= 0x2000         
            if carry == 1:
                X584[Pos] |= 0x4000
            Source += "\t" + Formats[op][0].format(V=V, A=A, B=B) + " (П={0})".format(carry) + "\n"
        else:
            Source += "\t" + Formats[op][0].format(V=V, A=A, B=B) + "\n"
        Pos += 1
    elif carry == 2:
        X584[Pos] |= 0x2000
        Source += "\t" + Formats[op][1].format(V=V, A=A, B=B) + "\n"
        Pos += 1
    elif carry == 3:
        X584[Pos] |= 0x6000
        Source += "\t" + Formats[op][2].format(V=V, A=A, B=B) + "\n"
        Pos += 1
        if Formats[op][0].find("- 1 + П") != -1:
            X584[Pos] = Opcode | 0x6000
            Source += "\t" + Formats[op][0].format(V=V, A=A, B=B).replace("- 1 + П", "- 1 + 1") + "\n"
            Pos += 1

def write_sum(opcode, carry):
    global X584, Pos, Source
    Opcode = opcode
    Cu, C0, C1 = Sum[opcode]
    if carry == 2 and C0 is None:
        return
    if carry == 3 and C1 is None:
        return
    if Opcode in Equiv:
        Opcode = Equiv[opcode]
    X584[Pos] = Opcode
    if carry == 0 or carry == 1:
        if Cu.find("П") != -1:
            X584[Pos] |= 0x2000         
            if carry == 1:
                X584[Pos] |= 0x4000
            Source += "\t" + Cu + " (П={0})".format(carry) + "\n"
        else:
            Source += "\t" + Cu + "\n"
        Pos += 1
    elif carry == 2:
        X584[Pos] |= 0x2000
        Source += "\t" + C0 + "\n"
        Pos += 1
    elif carry == 3:
        X584[Pos] |= 0x6000
        Source += "\t" + C1 + "\n"
        Pos += 1

def write_shift(opcode, carry):
    global X584, Pos, Source
    X, Op, Cu, C0, C1 = Shift[opcode]
    if carry == 2 and C0 is None:
        return
    if carry == 3 and C1 is None:
        return
    Opcode = opcode
    if Opcode in Equiv:
        Opcode = Equiv[opcode]
    X584[Pos] = Opcode
    if carry == 0 or carry == 1:
        if Cu.find("П") != -1:
            X584[Pos] |= 0x2000         
            if carry == 1:
                X584[Pos] |= 0x4000
        if X == 0:
            Source += "\t" + "РР := " + Op + "(" + Cu + ")" + " (П={0})".format(carry) + "\n"
        elif X == 1:
            Source += "\t" + "(РР, РРР) := " + Op + "(" + Cu + ", РРР)" + " (П={0})".format(carry) + "\n"
        Pos += 1
    elif carry == 2:
        if C0 is None:
            return
        X584[Pos] |= 0x2000
        if X == 0:
            Source += "\t" + "РР := " + Op + "(" + C0 + ")" + "\n"
        elif X == 1:
            Source += "\t" + "(РР, РРР) := " + Op + "(" + C0 + ", РРР)" + "\n"
        Pos += 1
    elif carry == 3:
        if C1 is None:
            return
        X584[Pos] |= 0x6000
        if X == 0:
            Source += "\t" + "РР := " + Op + "(" + C1 + ")" + "\n"
        elif X == 1:
            Source += "\t" + "(РР, РРР) := " + Op + "(" + C1 + ", РРР)" + "\n"
        Pos += 1

for mode in Modes:
    for op in Formats:
        write_alu(mode, op, 0)
    export(ModeNames[mode])
    for op in Formats:
        write_alu(mode, op, 1)
    export(ModeNames[mode])
    for op in Formats:
        write_alu(mode, op, 2)
    export(ModeNames[mode])
    for op in Formats:
        write_alu(mode, op, 3)
    export(ModeNames[mode])

for opcode in Sum:
    write_sum(opcode, 0)
export("sum-0-0")
for opcode in Sum:
    write_sum(opcode, 1)
export("sum-0-1")
for opcode in Sum:
    write_sum(opcode, 2)
export("sum-1-0")
for opcode in Sum:
    write_sum(opcode, 3)
export("sum-1-1")

for opcode in Shift:
    write_shift(opcode, 0)
export("shift-0-0")
for opcode in Shift:
    write_shift(opcode, 1)
export("shift-0-1")
for opcode in Shift:
    write_shift(opcode, 2)
export("shift-1-0")
for opcode in Shift:
    write_shift(opcode, 3)
export("shift-1-1")
