   M S B A M L   `   `   `  ????G  CWpf_Practice, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null+clr-namespace:Wpf_PracticeWpf_Practice  5   	   I)clr-namespace:Wpf_Practice.MVVM.ViewModelWpf_Practice.MVVM.ViewModel  5   	       Wpf_Practice.MainWindow   5   	   W SPresentationCore, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=6.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   ,localclr-namespace:Wpf_Practice     5   	   ?	viewModel)clr-namespace:Wpf_Practice.MVVM.ViewModel     5   	   $	??600??5	   	   $	??920??6     ? WindowStyle$
  None=?5
   	    ? 
ResizeMode$ 	CanResize=?5   	   
??????  ? AllowsTransparency$
 True??5   	    ? DataContext 5      /  )Wpf_Practice.MVVM.ViewModel.MainViewModel  5   
   .??5      ?? ???#272537 ?? CornerRadius$ 20??5      ._?5   
   ?  ? ColumnDefinitions 5      ?? 5      $	??200 ?6#   ?? 5       ? RowDefinitions 5      ?? 5      $??75 ?6    ?? 5      .+?5      ?? $??Elysian??6   $??Center=?5       $
??Left=?5!      
???????$??22(?5#      $??20,0,0,0q?5$      ?? 5&      $??1??6   .+?5(      	? $
??Home??6   $??50??5)      
???????$??14(?5+         MenuButtonTheme#??[  5,       d? 	IsChecked$
 TrueY?5-      ??5.      ?? *HomeViewCommand+	? 50      $??	Discovery??6   $??50??51      
???????$??14(?53      #??[  54      ??55      ?? *DiscoveryViewCommand+	? 57      $??Featured??6   $??50??58      
???????$??14(?5:      #??[  5;      5=      ?? 5?      $	??250??6   $??40??5@      $??Center=?5A      $
??Left=?5B      $??5q?5C      $??1??5D        ModernTextbox#??[ 5E      ?? 5G      $??1??6   $??1??5H      $??10q?5I      ??5J      ?? *CurrentView+5L      5N      5O      