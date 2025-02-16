class CSI_StaminaBar : SCR_InfoDisplay
{
	protected CSI_ClientComponent m_ClientComponent;
	protected ProgressBarWidget m_wStamBar;
	protected bool hudToggled = false;

	//------------------------------------------------------------------------------------------------

	// Override/static functions

	//------------------------------------------------------------------------------------------------

	protected override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);
		GetGame().GetInputManager().AddActionListener("RevealCSIUI", EActionTrigger.DOWN, ToggleIsVisible);
		GetGame().GetInputManager().AddActionListener("RevealCSIUI", EActionTrigger.UP, ToggleIsVisible);
	}

	//------------------------------------------------------------------------------------------------
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);

		if (!m_ClientComponent || !m_wStamBar) 
		{
			m_ClientComponent = CSI_ClientComponent.GetInstance();
			m_wStamBar = ProgressBarWidget.Cast(m_wRoot.FindWidget("StamBar"));
			return;
		};

		string stamBarVisible = m_ClientComponent.ReturnLocalCSISettings()[3];
		string hudAutoHidden = m_ClientComponent.ReturnLocalCSISettings()[14];

		if (stamBarVisible == "false" || (hudAutoHidden == "true" && !hudToggled)) 
		{
			if (m_wStamBar.GetOpacity() > 0) m_wStamBar.SetOpacity(0);
			return;
		}

		// Get local enity the player is controlling at the moment.
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());

		// Can't run if these dont exist better exit out.
		if (!character) 
			return;

		// Get Charachter Controller of the locally played entity.
		SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));

		if (!characterController) 
			return;

		// Use local Charachter Controller to get the current players stamina, then use custom function OnStaminaChange() to show current stamina on players screen.
		OnStaminaChange(characterController.GetStamina());
	}

	//------------------------------------------------------------------------------------------------

	// Stamina Bar Functions

	//------------------------------------------------------------------------------------------------
	
	protected void ToggleIsVisible()
	{
		hudToggled = !hudToggled;
	}

	//------------------------------------------------------------------------------------------------
	void RevealBar(float currentOpacity)
	{
		float setOpacity = currentOpacity + 0.005;
		m_wStamBar.SetOpacity(setOpacity);
	}

	//------------------------------------------------------------------------------------------------
	protected void HideBar(float currentOpacity)
	{
		float setOpacity = currentOpacity -0.005;
		m_wStamBar.SetOpacity(setOpacity);
	}

	//------------------------------------------------------------------------------------------------
	void OnStaminaChange(float stamina)
	{
		m_wStamBar.SetCurrent(stamina); 

		// Color
		switch (true)
		{
			case (stamina < 0.7 && stamina > 0.3) : { m_wStamBar.SetColor(new Color(0.091997, 0.083009, 0.035996, 1.000000)); break;};
			case (stamina < 0.3)                  : { m_wStamBar.SetColor(new Color(0.188724, 0.046860, 0.037476, 1.000000)); break;};
			default                               : { m_wStamBar.SetColor(new Color(0.035996, 0.091997, 0.050004, 1.000000)); break;};
		};

		float currentOpacity = m_wStamBar.GetOpacity();

		// Opacity
		switch (true)
		{
			case (stamina < 1 && currentOpacity < 1)  : { RevealBar(currentOpacity); break;};
			case (stamina == 1 && currentOpacity > 0) : { HideBar(currentOpacity); break;};
		};
	}
}
