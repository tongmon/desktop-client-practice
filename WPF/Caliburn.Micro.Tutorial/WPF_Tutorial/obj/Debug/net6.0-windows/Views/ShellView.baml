   M S B A M L   `   `   `  ����G  CWPF_Tutorial, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null7 clr-namespace:WPF_Tutorial.ViewsWPF_Tutorial.Views  5   	   "    WPF_Tutorial.Views.ShellView   5   	   W SPresentationCore, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=6.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   2local clr-namespace:WPF_Tutorial.Views     5   	   ���#432352  � Title$  	ShellView��5	   	   $	��450��6   $	��800��6(    � WindowStartupLocation$ CenterScreen=�5
   	   .��5      �  � ColumnDefinitions 5   
   �� 5      $��20 �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $��* �6   �� 5      $��20 �6    � RowDefinitions 5   
   �� 5      $��20 �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $
��auto �6   �� 5      $��* �6   �� 5      $��20 �6   .+�5!   
   �� $��1��6@   $��1��6M   $��2��6]   ��6   ��  �� Path$ FullName� �� Mode$ OneWay=�6s   �� 5$   
   -   5         ��Name$ 	FirstName��5$   >   $	��100��6   $��2��6!   $��1��6.   6S   �� 5%   
   -   5        $ LastName��5%   >   $	��100��6   $��2��6!   $��2��6.   6R   �� 5(   
   -   5         ��Name$ People��5(   0   $��3��6   $��1��6     �� DisplayMemberPath$ 	FirstName��5*      	 �� SelectedItem	 5)      �� $ SelectedPerson�$ OneWayToSource=�5*      �� 5,   
   -   5        
 ��Name$
 SelectedPerson_LastName��5,   1   $��3��6   $��2��6!    �� TextAlignment$ Center=�6R   �� 50   
   -   5         ��Name$ 	ClearText��50      $��4��6$   $��1��61   .��6A   Clear Names6N   �� 53   
   -   5        $ LoadPageOne��53      $��5��6&   $��1��63   .��6C   Load First Page6T   �� 54   
   -   5        $ LoadPageTwo��54      $��5��6&   $��2��63   .��6C   Load Second Page6U   �� 57   
   -   5         ��Name$ 
ActiveItem��57   J   $��6��6   $��1��6&   $��5��66   6J   58      59      