#
# This is the user-interface definition of a Shiny web application. You can
# run the application by clicking 'Run App' above.
#
# Find out more about building applications with Shiny here:
# 
#    http://shiny.rstudio.com/
#

library(shiny)

# Define UI for application that draws a histogram
shinyUI(fluidPage(
  headerPanel("Network Parser", "NetParser"),
  tabsetPanel(
    tabPanel("tab1", plotOutput("myPlot")),
    tabPanel("tab2", "contents"),
    tabPanel("tab3", "contents")
  )
))
