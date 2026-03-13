package com.example.calculator

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.calculator.ui.theme.CalculatorTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            CalculatorTheme {
                CalculatorApp()
            }
        }
    }
}

@Composable
fun CalculatorApp() {
    Scaffold(
        modifier = Modifier.fillMaxSize(),
    ) { innerPadding ->
        CalculatorScreen(
            modifier = Modifier
                .background(Color(0xFF101216))
                .fillMaxSize()
                .padding(innerPadding)
        )
    }
}

/**
 * variables for the calculator.
 * display  -> what user sees
 * stored   -> number kept for next op
 * op       -> pending operator, like "+", "-", "*", "/"
 * overwrite -> if true, next digit replaces display
 */
private data class CalcState(
    val display: String = "0",
    val stored: Double? = null,
    val op: String? = null,
    val overwrite: Boolean = true

)

@Composable
fun CalculatorScreen(modifier: Modifier = Modifier) {
    var state by remember { mutableStateOf(CalcState()) }
    val backspace = {  s: String ->
        if (s.length <= 1)
            "0"
         else
            s.dropLast(1)

    }
    // --- core math, kept tiny on purpose ---
    fun eval(a: Double, b: Double, op: String): Double =
        when (op) {
            "+" -> a + b
            "-" -> a - b
            "*" -> a * b
            "/" -> a / b     // you can guard 0 later
            else -> b        // fallback
        }

    fun format(n: Double): String =
        if (n % 1.0 == 0.0) n.toLong().toString()
        else n.toString()

    // --- input handler, all paths go through here ---
    fun onButton(label: String) {
        when (label) {
            // digits
            in listOf("0", "1", "2", "3", "4", "5", "6", "7", "8", "9") -> {
                if (state.display == "Error") return
                val next = if (state.overwrite || state.display == "0") label
                else state.display + label
                state = state.copy(display = next, overwrite = false)
            }

            // decimal
            "." -> {
                if (state.display == "Error") return
                if (state.overwrite) {
                    state = state.copy(display = "0.", overwrite = false)// if displaying 0 then 0.
                } else if (!state.display.contains('.')) {// if theres already a . then another one isnt allowed
                    state = state.copy(display = state.display + ".")
                }
            }

            // clear
            "AC" -> state = CalcState("0",null,null,true)
            "C" -> state = CalcState( "0",  state.stored, state.op, true)


            "⌫" -> state = CalcState( backspace(state.display),  state.stored, state.op, true)
            "+/-" -> {
                // don't change Error
                if (state.display == "Error") return

                // don't bother changing 0
                if (state.display == "0") return

                val num = state.display.toDoubleOrNull() ?: return
                val neg = num * -1
                state = state.copy(
                    display = if (neg % 1.0 == 0.0) neg.toLong().toString() else neg.toString()
                )
            }

            // equals
            "=" -> {
                if (state.display == "Error") return
                val b = state.display.toDoubleOrNull() ?: return
                val a = state.stored
                val op = state.op
                var res = 0.0
                if (a != null && op != null) {
                    if (op == "/" && b == 0.0)
                        state = state.copy(
                            display = "Error",
                            stored = null,
                            op = null,
                            overwrite = true
                        )
                    else {
                        res = eval(a, b, op)
                        state = CalcState(display = format(res))
                    }
                }
            }

            // operators
            in listOf("+", "-", "×", "÷") -> {
                if (state.display == "Error") return
                val current = state.display.toDoubleOrNull() ?: 0.0
                val opSymbol = when (label) {
                    "×" -> "*"
                    "÷" -> "/"
                    else -> label
                }


                if (state.stored != null && state.op != null && !state.overwrite) {
                    val prev = state.stored!!
                    val prevOp = state.op!!
                    // protect divide by 0 here too
                    if (prevOp == "/" && current == 0.0) {
                        state = state.copy(
                            display = "Error",
                            stored = null,
                            op = null,
                            overwrite = true
                        )
                    } else {
                        val res = eval(prev, current, prevOp)
                        state = state.copy(
                            display = format(res),
                            stored = res,        // so you can keep doing +, -...
                            op = opSymbol,       // update to the new op just pressed
                            overwrite = true
                        )
                    }
                } else {
                    // first op or op after result
                    state = state.copy(
                        stored = current,
                        op = opSymbol,
                        overwrite = true
                    )
                }

            }
        }
    }

    Column(
        modifier = modifier.padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(12.dp)
    ) {
        // display
        Text(
            text = state.display,
            color = Color.White,
            fontSize = 48.sp,
            fontWeight = FontWeight.SemiBold,
            modifier = Modifier
                .fillMaxWidth()
                .padding(8.dp),
            textAlign = TextAlign.End
        )

        // keypad
        val keys = listOf(
            listOf("AC", "C", "⌫", "÷"),
            listOf("7", "8", "9", "×"),
            listOf("4", "5", "6", "-"),
            listOf("1", "2", "3", "+"),
            listOf(".","0", "+/-","=")
        )

        keys.forEach { row ->
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                row.forEach { label ->
                    val weight = if (label == "=") 1.5f else 1f
                    CalcButton(
                        label = label,
                        modifier = Modifier
                            .weight(weight)
                            .height(64.dp),
                        onClick = { onButton(label) },
                        containerColor = when (label) {
                            "+", "-", "×", "÷", "=" -> Color(0xFFD52F2F)
                            "AC" -> Color(0xFF2D3436)
                            else -> Color(0xFF1E2227)
                        }
                    )
                }
                // pad last row if needed
                if (row.size < 4) {
                    repeat(4 - row.size) {
                        Spacer(modifier = Modifier.weight(1f).height(64.dp))
                    }
                }
            }
        }
    }
}


@Composable
private fun CalcButton(
    label: String,
    onClick: () -> Unit,
    modifier: Modifier = Modifier,
    containerColor: Color = Color(0xFF1E2227)
) {
    Button(
        onClick = onClick,
        modifier = modifier,
        colors = ButtonDefaults.buttonColors(containerColor = containerColor),
        shape = RoundedCornerShape(16.dp),
        elevation = ButtonDefaults.buttonElevation(defaultElevation = 2.dp)
    ) {
        Text(
            text = label,
            color = Color.White,
            fontSize = 22.sp,
            modifier = Modifier.fillMaxWidth(),
            textAlign = TextAlign.Center
        )
    }
}

@Preview(showBackground = true)
@Composable
fun CalculatorPreview() {
    CalculatorTheme { CalculatorApp() }
}
