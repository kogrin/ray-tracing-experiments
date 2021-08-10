import terminaltables

var t2 = newUnicodeTable()
t2.separateRows = false
t2.setHeaders(@[newCell("ID", pad=5), newCell("Name", rightpad=10), newCell("Fav animal", pad=2), newCell("Date", 5)])
t2.addRow(@["1", "xmonader", "Cat, Dog", "2018-10-22"])
t2.addRow(@["2", "ahmed", "Shark", "2015-12-6"])
t2.addRow(@["3", "dr who", "Humans", "1018-5-2"])
t2.separateRows = true
let testStyle =  Style(rowSeparator:"┈", colSeparator:"┇", cellEdgeLeft:"├", cellEdgeRight:"┤", topLeft:"┏", topRight:"┓", bottomLeft:"└", bottomRight:"┘", topRowSeparator:"┬", bottomRowSeparator:"┴", dashLineLeft:"├", dashLineRight:"┤", dashLineColSeparatorLastRow:"┴", dashLineColSeparatorTopRow:"┬", dashLineColSeparator:"┼")

t2.style = testStyle
printTable(t2)