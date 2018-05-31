from django.urls import path
from  . import views


app_name = 'myalign'
urlpatterns = [
    path('', views.index, name = 'index'),
    path('add', views.add, name = 'add'),
    path('batchPage', views.batchPage, name = 'batchPage'),
    path('batchadd', views.batchadd, name = 'batchadd'),
    path('interPage', views.interPage, name = 'interPage'),
    path('interadd', views.interadd, name = 'interadd'),
    path('interaddword', views.interaddword, name = 'interaddword'),
]