#
# This is the server logic of a Shiny web application. You can run the 
# application by clicking 'Run App' above.
#
# Find out more about building applications with Shiny here:
# 
#    http://shiny.rstudio.com/
#

var_n = 1

library(shiny)
library(reshape2)

dyn.load("../interface.so")

.Call("startSession")
onStop(function() {
  cat("ending Session\n")
  .Call("endSession")
})

# Define server logic required to draw a histogram
shinyServer(function(input, output) {
   
  getData = reactive({
    invalidateLater(1000)
    df=data.frame(time=1:var_n,y=runif(var_n))
    var_n <<- var_n + 5
    print(var_n)
    df = log(df)
    return(df)
  })
  
  getRealData = reactive({
    invalidateLater(1000)
    df <- data.frame(.Call("getSecondData"))
    #print(colnames(df))
    #print(df$myColumn4)
    #print(df)
    if (sum(df) > 0) {
      df = log(df)
    }
    df$time = 1:60
    return(df)
  })
  
  output$myPlot <- renderPlot({
    
    # generate bins based on input$bins from ui.R
    df = getRealData()
    df = melt(df, id.vars = 'time', variable.name = 'source')
    
    par(mar=c(5.1, 4.1, 4.1, 8.1), xpd=TRUE)
    plot(df$time, df$value, col=df$source, xlab = "Time (seconds)", ylab = "Ln Frequency (bytes/second)", pch = 23, cex = 1.5)
    legend("right", inset=c(-0.17,0), legend = c("(failed lookup)", "google.com", "facebook.com", "amazonaws.com", "googleusercontent.com", "illinois.edu", "github.com"), col=1:length(df$source), pch = 1)
    #ggplot(df, aes(time, value)) + geom_line(aes(color = source))

  }, width = 800)
  
  getData1 = reactive({
    invalidateLater(500 * 5)
    df=data.frame(time=1:var_n,y=rnorm(var_n))
    var_n <<- var_n + 1
    print(var_n)
    return(df)
  })
  
  output$myPlot1 <- renderPlot({
    df = getData1()
    
  }, width = 800)
  
})
