package main

import (
	"bufio"
	"flag"
	"fmt"
	"os"
	"strconv"
	"unicode"
)

var DEBUG bool = false
var BLANK_CHARACTER rune

type Table struct {
	// The table's 2D matrix
	RawTable [][]rune

	// The Values in the table
	Values []TableValue

	// The Symbols in the table
	Symbols []TableSymbol
}

// The numbers present in the table
type TableValue struct {
	// The numeric value
	Val int

	// The leftmost x-coordinate, starting at 0
	X1 int

	// The rightmost x-coordinate, starting at 0
	X2 int

	// The Y-coordinate, starting at 0
	Y int
}

// The symbols present in the table
type TableSymbol struct {
	// The string representation
	Val string

	// The X-coordinate, starting at 0
	X int

	// The Y-coordinate, starting at 0
	Y int
}

func init() {
	if _, isSet := os.LookupEnv("DEBUG"); isSet {
		fmt.Println("DEBUG Mode Enabled")
		DEBUG = true
	}

	const (
		defaultBlank = "."
		usage        = "The character used to signify blank spaces in the table"
	)
	var blankInput string
	flag.StringVar(&blankInput, "blank_character", defaultBlank, usage)
	flag.StringVar(&blankInput, "b", defaultBlank, usage+" (shorthand)")
	if runes := []rune(blankInput); len(runes) == 1 {
		BLANK_CHARACTER = runes[0]
	} else {
		fmt.Println("Warning: --blank_character (-b) has to be a single character")
	}
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)

	fmt.Println("Write the input table (Ctrl+D or Ctrl+Z to finish):")

	lines := []string{}
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "Reading standard input:", err)
	}

	if DEBUG {
		for line := range lines {
			fmt.Printf("Line %v: %s\n", line, lines[line])
		}
	}

	table, err := ParseTable(lines)
	if err != nil {
		fmt.Fprintln(os.Stderr, "Failed to parse table:", err)
	}

	// TABLE PARSED, DO WHAT YOU MUST!
	// (An example of my own dev experience taking over efficiency, but it doesn't need to be efficient anyways)

	numberSum := uint64(0)
	// Part 1
	// The efficiency here will be kinda awful, but I don't feel like using the raw table
	// for _, val := range table.Values {
	// 	for _, sym := range table.Symbols {
	// 		if sym.X >= val.X1 - 1 && sym.X <= val.X2 + 1 && sym.Y >= val.Y - 1 && sym.Y <= val.Y + 1 {
	// 			if DEBUG {
	// 				fmt.Printf("%v is adjacent to %s\n", val.Val, sym.Val)
	// 			}
	// 			numberSum += val.Val
	// 			break
	// 		}
	// 	}
	// }

	// Part 2
	// The efficiency here will be kinda awful here too, but I don't feel like using the raw table here either
	for _, sym := range table.Symbols {
		if sym.Val == "*" {
			nums := []int{}
			for idx, val := range table.Values {
				if sym.X >= val.X1-1 && sym.X <= val.X2+1 && sym.Y >= val.Y-1 && sym.Y <= val.Y+1 {
					if DEBUG {
						fmt.Printf("%s at (%v,%v) is adjacent to %v\n", sym.Val, sym.X, sym.Y, val.Val)
					}
					nums = append(nums, val.Val)
				}

				if idx == len(table.Values)-1 {
					if len(nums) == 2 {
						numberSum += uint64(nums[0] * nums[1])
					}
				}
			}
		}
	}

	fmt.Printf("The sum of valid values is: %v\n", numberSum)
}

// Parses a table, given as an array of text lines.
//
// Also currently it never returns a non-nil error... I just added it because of muscle memory
func ParseTable(rawInput []string) (Table, error) {
	results := Table{
		RawTable: [][]rune{},
	}

	for line := range rawInput {
		lineRunes := []rune{}
		for _, char := range rawInput[line] {
			lineRunes = append(lineRunes, char)
		}
		results.RawTable = append(results.RawTable, lineRunes)
	}

	// Yes, it's a double pass
	for y := range results.RawTable {
		reading := false
		currentValString := ""
		currentValStart := 0
		for x, char := range results.RawTable[y] {
			if char == BLANK_CHARACTER {
				if reading {
					// WARN: Yeah we are ignoring any string to int error
					val, _ := strconv.Atoi(currentValString)
					results.Values = append(results.Values, TableValue{
						Val: val,
						X1:  currentValStart,
						X2:  x - 1,
						Y:   y,
					})
					reading = false
					currentValString = ""
					currentValStart = 0
				}
				continue
			}

			// Check if it's a digit
			if unicode.IsDigit(char) {
				if reading {
					currentValString = currentValString + string(char)
				} else {
					reading = true
					currentValString = currentValString + string(char)
					currentValStart = x
				}

			} else { // Otherwise it's a symbol
				// Finish reading if currently reading
				if reading {
					// WARN: Yeah we are ignoring any string to int error
					val, _ := strconv.Atoi(currentValString)
					results.Values = append(results.Values, TableValue{
						Val: val,
						X1:  currentValStart,
						X2:  x - 1,
						Y:   y,
					})
					reading = false
					currentValString = ""
					currentValStart = 0
				}

				results.Symbols = append(results.Symbols, TableSymbol{
					Val: string(char),
					X:   x,
					Y:   y,
				})
			}

			// Check if it's the last character in the line
			if x == len(results.RawTable[y])-1 {
				if reading {
					// WARN: Yeah we are ignoring any string to int error
					val, _ := strconv.Atoi(currentValString)
					results.Values = append(results.Values, TableValue{
						Val: val,
						X1:  currentValStart,
						X2:  x,
						Y:   y,
					})
				}

			}
		}
	}

	return results, nil
}
