function test_activex(){
	//������Ϸ�ؼ�
	bbb = new ActiveXControl("Tank90Control.Tank90", -2)
	bbb.Ax_MoveWindow(50, 50, 600, 600)
	bbb.Ax_ShowWindow(1)
	//external.Log(bbb.Ax_ListSym())

	//�Զ���ʼ��Ϸ
	bbb.StartGame()
}

setTimeout("test_activex()", 6000)
