#
# This is the server logic of a Shiny web application. You can run the 
# application by clicking 'Run App' above.
#
# Find out more about building applications with Shiny here:
# 
#    http://shiny.rstudio.com/
#

var_n = 5

library(shiny)

# Define server logic required to draw a histogram
shinyServer(function(input, output) {
   
  getData = reactive({
    invalidateLater(500)
    df=data.frame(time=1:var_n,y=runif(var_n))
    var_n <<- var_n + 5
    print(var_n)
    return(df)
  })
  
  output$myPlot <- renderPlot({
    
    # generate bins based on input$bins from ui.R
    df = getData()
    plot(df$time, df$y)
    
  
  })
  
  getData1 = reactive({
    invalidateLater(500 * 5)
    df=data.frame(time=1:var_n,y=rnorm(var_n))
    var_n <<- var_n + 5
    print(var_n)
    return(df)
  })
  
  output$myPlot1 <- renderPlot({
    df = getData1()
    plot(df$time, df$y)
  })
  
})
