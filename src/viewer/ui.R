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
    tabPanel("Real-Time plot", verticalLayout(
      plotOutput("myPlot")
      ))
)))
