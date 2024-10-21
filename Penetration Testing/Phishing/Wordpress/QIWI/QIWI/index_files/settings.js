







if (typeof settings == "undefined") {
	settings = {};
}

settings.acquiring = {
	subscribe : {
		link : "/acquiring/content/subscribe/process.action",
		form : {
			main : {
				link : "/acquiring/content/subscribe/form.action",
				path : ""
			}
		}
	},
	pay : {
		subscribed : {
			process : {
				link : "/acquiring/content/pay/subscribed/process.action"
			},
			form : {
				link : "/acquiring/content/pay/subscribed/form.action",
				path : ""
			}
		},
		unsubscribed : {
			process : {
				link : "/acquiring/content/pay/unsubscribed/process.action"
			},
			form : {
				link : "/acquiring/content/pay/unsubscribed/form.action",
				path : ""
			}
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.anelik = {
	remitent : {
		link : "/anelik/content/remitent.action"
	},
	newrecipient : "New recipient"
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.applications = {
	main : {
		main : {
			link : "/applications/content/main.action",
			path : "/settings/apps/main.action",
			name : ""
		},
		all : {
			link : "/applications/content/client.action",
			path : "/settings/apps/all.action",
			name : ""
		},
		client : {
			link : "/applications/content/client.action",
			path : "/settings/apps/client.action",
			name : ""
		},
		category : {
			link : "/applications/content/category.action",
			path : "/settings/apps/category.action",
			name : ""
		}
	},
	menu : {
		link : "/applications/content/leftmenu.action"
	},
	promo : {
		main : {
			link : "/applications/content/promo.action"
		}
	},
	suggest : {
		main : {
			link : "/applications/content/suggest.action"
		}
	},
	list : {
		main : {
			link : "/user/applications/content/list.action",
			path : "/settings/apps/mine.action",
			name : ""
		},
		lock : {
			token : {
				link : "/user/oauth/reject.action",
				confirm : "Вы действительно хотите заблокировать код доступа?"
			},
			client : {
				link : "/user/oauth/reject.action",
				confirm : "Вы действительно хотите заблокировать все коды доступа?"
			}
		}
	},
	oauth : {
		form : {
			main : {
				link : "/oauth/content/form/confirm.action",
				path : "/oauth/confirm.action",
				name : ""
			},
			allow : {
				link : "/oauth/content/form/sms.action"
			},
			code : {
				link : "/oauth/content/form/code.action",
				path : "/oauth/code.action",
				name : ""
			},
			confirm : {
				link : "/oauth/content/form/submit.action"
			}
		},
		login : {
			form : {
				link : "/oauth/content/form/confirm.action",
				path : "/oauth/confirm.action"
			}
		},
		remind : {
			form : {
				link : "/external/remind/content/form.action",
				path : "/oauth/remind/form.action"
			},
			password : {
				link : "/external/remind/content/password.action",
				path : "/oauth/remind/password.action"
			},
			mail : {
				link : "/external/remind/content/mail.action",
				path : "/oauth/remind/mail.action"
			},
			success : {
				link : "/oauth/content/form/confirm.action",
				path : "/oauth/confirm.action"
			}
		},
		registration : {
			form : {
				link : "/external/register/content/form.action",
				path : "/oauth/register/form.action"
			},
			password : {
				link : "/external/register/content/password.action",
				path : "/oauth/register/password.action"
			},
			result : {
				link : "/oauth/content/form/confirm.action",
				path : "/oauth/confirm.action"
			}
		}
	},
	mobile : {
		main : {
			link : "/mobile/notification/main.action"
		},
		answer : {
			link : "/mobile/notification/answer.action"
		}
	}
};




if (typeof settings == "undefined") {
	settings = {};
}

settings.qiwi = {
	context : "",
	static : "//static.qiwi.com",
	locale : "ru",
	google : {
	    api : {
	        key : "AIzaSyBscynRg7X1D9b5Te-rdsWRTVUtd4-qWpI"
        }
    },
    sso : {
        enabled : true,
        server : "https:\/\/sso.qiwi.com",
        check : "/j_spring_cas_security_check"
    },
	main : {
		link : "/banners/main.tile",
		path : "/main.action",
		name : "Visa QIWI Wallet"
	},
	empty : {
		link : "/system/empty.action",
		path : "/empty.action"
	},
	logout : {
		link : "/auth/logout.action"
	},
	login : {
		link : "notsupported",
		form : {
			link : "/auth/content/form.action",
			path : "/auth/form.action",
			name : ""
		},
		captcha : {
			link : "/auth/captcha.action"
		}
	},
	registration : {
		link : "/register/main.action",
		form : {
			link : "/register/content/form.action",
			path : "/register/form.action",
			name : "Регистрация"
		},
		captcha : {
			link : "/register/captcha.action"
		},
		oferta : {
			link : "/register/oferta.action"
		},
		result : {
			link : "/auth/content/form.action",
			path : "/auth/form.action"
		},
		password : {
			link : "/register/content/password.action",
			path : "/register/password.action"
		}
	},
	remind : {
		link : "/remind/main.action",
		captcha : {
			link : "/remind/captcha.action"
		},
		send : {
			link : "/person/email/remind/content/message.action"
		},
		form : {
			link : "/remind/form/content.action",
			path : "/remind/form.action",
			name : "Восстановление пароля"
		},
		password : {
			link : "/remind/password/content.action",
			path : "/remind/password.action"
		},
		mail : {
			link : "/person/email/content/sendsuccess.action",
			path : "/person/email/sendsuccess.action"
		},
		code : {
			link : "/person/email/remind/content/confirm.action",
			path : "/person/email/remind/confirm.action"
		},
		success : {
			link : "/person/change/success/content/password.action",
			path : "/person/change/success/password.action",
			name : ""
		}
	},
	country : {
		select : {
			link : "/menu/countries.action"
		}
	},
	menu : {
		main : {
			link : "/menu/set.tile?menuType=TOP"
		},
		additional : {
			link : "/menu/set.tile?menuType=NAV"
		},
		breadcrumbs : {
			link : "/provider/content/categorypath.action"
		}
	},
	provider : {
		redirect : {
			link : "/provider/redirect.action"
		},
		top : {
			link : "/provider/topmenu.action?view=public"
		}
	},
    oauth : {
        label : "Или войдите через:",
        or : "или",
        bind : "Добавить",
        unbind : "Отключить",
        labelsocial : "Социальные сети",
        disabled : "Авторизация через социальную сеть невозможна. Отключите, пожалуйста, блокировку всплывающих окон.",
		provider : {
            vkontakte : "Войти через Вконтакте",
            facebook : "Войти через Facebook",
            google : "Войти через Google"
        },
        bindprovider : {
            vkontakte : "Вконтакте",
            facebook : "Facebook",
            google : "Google+"
        },
        unbindprovider : {
            vkontakte : "Вконтакте  (привязан)",
            facebook : "Facebook (привязан)",
            google : "Google+  (привязан)"
        },
        login : {
            form : {
                link : "/oauth2/content/form.action",
                path : "/oauth2/form.action"
            }
        }
    }
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.card = {
	menu : {
		main : {
			link : "/card/menu.action"
		},
		remove : {
			link : "/user/card/remove.action",
			confirm : "Вы уверены, что хотите удалить карту?"
		}
   	},
	create : {
		main : {
			link : "/user/card/content/main.action",
			path : "/settings/account/form/card.action"
		},
        form : {
            link : "/user/card/add.action",
            path : "/settings/account/form/card/add.action"
        },
		success : {
			link : "/card/add/content/success.action"
		},
		error : {
			link : "/card/add/content/error.action"
		}
	},
	update : {
		main : {
			link : "/user/card/edit.action",
			path : "/card/edit.action",
			name : ""
		},
		update : {
			link : "/user/card/save.action"
		},
		success : {
			link : "/card/content/success.action",
			path : "/card/success.action",
			name : ""
		}
	},
	confirm : {
		alfabank : {
			main : {
				link : "/card/confirm/alfabank.action",
				path : "/card/alfabank.action",
				name : ""
			}
		}
	},
	form  : {
		main : {
			link : "/card/content/choose.action",
			path : "/card/choose.action",
			name : ""
		},
		detect : {
			link : "/card/detect.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.cash = {
	credits : {
		main : {
			link : "/cash/json/credits.action",
			path : "/cash/credits.action",
			tmpl : "provider.list",
			name : ""

		}
	},
	transfers : {
		main : {
			link : "/cash/content/transfers.action",
			path : "/cash/transfers.action",
			name : ""
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.confirmation = {
	form : {
		link : "/user/confirmation/form.action"
	},
	confirm : {
		link : "/user/confirmation/confirm.action"
	},
	resend : {
		link : "/user/confirmation/resend.action",
		status : "Повторно запросить код можно будет через %s %s",
		unit : "сек."
	},
	public : {
		form : {
			link : "/confirmation/form.action"
		},
		confirm : {
			link : "/confirmation/confirm.action"
		},
		resend : {
			link : "/confirmation/resend.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.external = {};


if (typeof settings == "undefined") {
    settings = {};
}

settings.favorite = {
    remove : {
        link : "/user/favorite/delete.action",
        confirm : "Удалить эту запись?"
    },
    menu : {
        main : {
            link : "/favorite/leftmenu.action"
        }
    },
    list : {
        main : {
            link : "/user/favorite/list.action",
            path : "/payment/favorite.action"
        }
    },
    save : {
        main : {
            link : "/user/favorite/savepayment.action",
            path : "/payment/favorite/save.action",
            name : ""
        }
    },
    open : {
        main : {
            link : "/user/favorite/open.action",
            path : "/payment/favorite/open.action",
            name : ""
        }
    },
    task : {
        main : {
            link : "/user/favorite/task.action",
            path : "/payment/favorite/task.action",
            name : ""
        }
    },
    edit : {
        main : {
            link : "/user/favorite/edit.action",
            path : "/payment/favorite/edit.action",
            name : ""
        }
    },
    reset : {
        link: "/user/favorite/reset.action"
    }
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.feature = {
	list : {
		list : {
			link : "/features/content/list.action",
			path : "/features/list.action",
			name : ""
		},
		top : {
			link : "/features/content/top.action",
			path : "/features/top.action",
			name : ""
		},
		bonuses : {
			list : {
				link : "/bonuses/content/list.action",
				path : "/bonuses/list.action",
				name : ""
			},
			fixed : {
				link : "/bonuses/content/fixed.action",
				path : "/bonuses/fixed.action",
				name : ""
			},
			active : {
				link : "/bonuses/content/active.action",
				path : "/bonuses/active.action",
				name : ""
			},
			archive : {
				link : "/bonuses/content/archive.action",
				path : "/bonuses/archive.action",
				name : ""
			}
		},
		random : {
			link : "/bonuses/content/random.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.leader = {
	card : {
		link : "/leader/content/card.action"
	},
	newrecipient : "Новый получатель"
};


if (typeof settings == "undefined") {
    settings = {};
}

settings.megafon = {
    main : {
        link : "/user/megafon/content/main.action",
        path : "/megafon.action"
    },
    phone : {
        attach : {
            main : {
                link : "/user/megafon/content/form.action",
                path : "/megafon/form.action"
            },
            save : {
                link : "/user/megafon/content/save.action"
            },
            success : {
                link : "/user/megafon/content/success.action"
            }
        },
        list : {
            link : "/user/megafon/content/linkedphoneslist.action",
            main : {
                link : "/user/megafon/content/linkedphoneslistmain.action"
            }
        },
        update : {
            link : "/user/megafon/content/edit.action"
        },
        remove : {
            confirm : "Вы уверены, что хотите отвязать телефон?",
            link : "/user/megafon/content/remove.action"
        }
    },
    register : {
        confirm : {
            link : "/user/megafon/content/sendsms.action"
        },
        success : {
            link : "/auth/refresh.action"
        },
        confirmation : {
            form : {
                link : "/user/megafon/content/confirmation.action"
            }
        }
    },
    balance : {
        main : {
            link : "/user/megafon/content/balance.action"
        },
        header : {
            link : "/user/megafon/content/balanceheader.action"
        },
        source : {
            link : "/user/megafon/content/balancesource.action"
        }
    },
    feedback : {
        link : "/megafon/request.action"
    }
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.message = {
	list : {
		main : {
			link : "/user/message/content/list.action",
			path : "/lenta/messages.action",
			name : ""
		},
		more : {
			link : "/user/message/content/loadlist.action"
		},
		remove : {
			link : "/user/message/remove.action",
			confirm : {
				item : "Вы уверены, что хотите удалить сообщение?",
				selected : "Вы уверены, что хотите удалить выбранные сообщения?"
			}
		},
		mark : {
			link : "/user/message/mark.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.mobile = {
	form  : {
		main : {
			link : "/mobile/content/main.action",
			path : "/mobile/main.action",
			name : ""
		},
		detect : {
			link : "/mobile/detect.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.moscowrostelecom = {
	online : {
		info : {
			link : "/moscowrostelecom/information.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.news = {
	list : {
		main : {
			link : "/news/content/list.action",
			path : "/lenta/news.action",
			name : "Новости"
		}
	},
	item : {
		main : {
			link : "/news/content/item.action",
			path : "/lenta/news/item.action",
			name : ""
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.offer = {
	main : {
		link : "/user/offer/show.action"
	},
	shown : {
		link : "/user/offer/shown.action"
	},
	click : {
		link : "/user/offer/click.action"
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.order = {
	list : {
		link : "/user/order/list.action",
		path : "/order/list.action",
		name : ""
	},
	fragment: {
		link : "/content/fragment/order.action?type=3&size=5"
	},
	item : {
		reject : {
			link : "/user/order/reject.action",
			confirm : "Вы уверены, что хотите отменить счет?"
		},
		accept : {
			link : "/user/order/accept.action"
		},
		checkout: {
			link: "https://bill.qiwi.com/order/external/main.action"
		},
		payment : {
			form : {
				link : "/user/order/payment.action"
			}
		},
		status : {
			error : {
				image : {
					src : "\/img\/ui\/payError.png",
					alt : "Ошибка проведения платежа"
				}
			},
			processed : {
				image : {
					src : "\/img\/ui\/payWaiting.png",
					alt : "Платеж производится..."
				}
			},
			success : {
				image : {
					src : "\/img\/ui\/paySuccess.png",
					alt : "Платеж проведен успешно"
				}
			}
		}
	},
	menu : {
		link : "/order/leftmenu.action"
	},
	form : {
		result : {
			link : "/user/order/content/result.action",
			path : "/order/result.action"
		},
		create : {
			link : "/user/order/create.action"
		}
	},
	external : {
        main : {
            link : "/order/external/content/form/sources.action",
            path : "/order/external/main.action"
        },
        sources : {
            link : "/order/external/content/form/sources.action",
            path : "/order/external/main.action"
        },
		form : {
			main : {
				link : "/order/external/content/form/login.action",
				path : "/order/external/form.action"
			},
			accept : {
				link : "/order/external/accept.action",
				success : {
					link : "/order/external/content/form/success.action",
					path : "/order/external/success.action"
				},
				failure : {
					link : "/order/external/content/form/failure.action",
					path : "/order/external/failure.action"
				},
				phone : {
                    link : "/order/phone/accept.action",
					success : {
						link : "/order/external/phone/content/form/success.action",
						path : "/order/external/phone/success.action"
					},
					failure : {
						link : "/order/external/content/form/failure.action",
						path : "/order/external/failure.action"
					}
				},
                terminal : {
                    link : "/order/external/content/form/terminal.action",
                    path : "/order/external/terminal.action"
                }
			}
		},
		login : {
			form : {
				link : "/order/external/content/form/login.action",
				path : "/order/external/form.action"
			}
		},
		remind : {
			form : {
				link : "/external/remind/content/form.action",
				path : "/order/external/remind/main.action"
			},
			password : {
				link : "/external/remind/content/password.action",
				path : "/order/external/remind/password.action"
			},
			mail : {
				link : "/external/remind/content/mail.action",
				path : "/order/external/remind/mail.action"
			},
			success : {
				link : "/order/external/content/form/login.action",
				path : "/order/external/form.action"
			}
		},
		registration : {
			form : {
				link : "/external/register/content/form.action",
				path : "/order/external/register/form.action"
			},
			password : {
				link : "/external/register/content/password.action",
				path : "/order/external/register/password.action"
			},
			result : {
				link : "/order/external/content/form/login.action",
				path : "/order/external/form.action"
			}
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.payment = {
	main : {
        link : "/provider/categorylist.tile",
        path : "/payment/list.action"
	},
	state : {
		link : "/user/payment/form/state.action",
		path : "/payment/state.action",
		name : ""
	},
	protectedstate : {
		link : "/user/payment/form/state.action",
		path : "/payment/state.action",
		name : ""
	},
	form : {
		main : {
			link : "/payment/form/start.action",
			path : "/payment/form.action",
			name : ""
		},
		calculate : {
			link : "/payment/form/calculate.action"
		},
		acquiring : {
			link : "/payment/form/acquiring.action"
		},
		cardnumber : {
			link : "/card/info.action"
		},
		state : {
			link : "/payment/form/state.action"
		}
	},
	online : {
		info : {
			link : "/payment/online/info.action"
		}
	},
    amount : "{0} {1} {2} {3}"
};

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.card = {
	account : {
		link : "/payment/cash/cardcheck.action"
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.person = {
	login : {
		form : {
			link : ""
		}
	},
	info : {
		main : {
			link : "/person/menu.action"
		},
		state : {
			link : "/person/state.action"
		}
	},
	password : {
		main : {
			link : "/user/person/change/form/password.action",
			path : "/settings/options/password.action"
		},
		change : {
			link : "/user/person/change/password.action",
			success : {
				link : "/person/change/success/content/password.action",
				path : "/settings/options/password/success.action"
			}
		}
	},
	wallet : {
		main : {
			link : "/user/person/wallet/view.action",
			path : "/settings/options/wallet.action"
		},
		form : {
			main : {
				link : "/user/person/wallet/edit.action",
				path : "/settings/options/wallet/edit.action"
			},
			save : {
				link : "/user/person/wallet/save.action"
			}
		},
		sms : {
			toggle : {
				link: "/user/person/notification/save.action",
			},
			dismiss : "Если решите подключить услугу, вы найдете ее в настройках",
			enable : {
				message : "\u003Cdiv class=\"base-popup\"\u003E\n\u003Cp class=\"base-popup-title\"\u003EПодтвердите действие\u003C\/p\u003E\n\u003Cp class=\"base-popup-info\"\u003EХотите подключить услугу SMS оповещения за 29 рублей в месяц?\u003C\/p\u003E\n\u003C\/div\u003E",
				yes : "Подключить",
				no : "Отмена"
			},
			disable : {
				message : "\u003Cdiv class=\"base-popup\"\u003E\n\u003Cp class=\"base-popup-title\"\u003EПодтвердите действие\u003C\/p\u003E\n\u003Cp class=\"base-popup-info\"\u003EОтключить все уведомления?\u003C\/p\u003E\n\u003C\/div\u003E",
				yes : "Отключить уведомления",
				no : "Отмена"
			},
			enabled : "\u003Ch4 class=\"notification-dialog-title\"\u003EПодключение SMS-оповещений\u003C\/h4\u003E\n\u003Cp class=\"notification-dialog-info\"\u003EВы будете получать SMS о пополнениях и расходах.\u003C\/p\u003E",
			disabled : "\u003Ch4 class=\"notification-dialog-title\"\u003EОтключение SMS-оповещений\u003C\/h4\u003E\n\u003Cp class=\"notification-dialog-info\"\u003EВы не будете получать SMS о пополнениях кошелька и расходах с 15 октября.\u003C\/p\u003E",
			close : "Ок"
		},
		premium : {
		    prolongation : {
				toggle : {
					link : "/user/person/premium/autoProlongation.action"
				},
				disable : {
					message : "\u003Cdiv class=\"base-popup\"\u003E\n\u003Cp class=\"base-popup-title\"\u003EПодтвердите действие\u003C\/p\u003E\n\u003Cp class=\"base-popup-info\"\u003EОтключить автопродление пакета \"Приоритет\"?\u003C\/p\u003E\n\u003C\/div\u003E",
					yes : "Отключить автопродление",
					no : "Отмена"
				}
			}
		}
	},
	confirmation : {
		main : {
			link : "/user/person/change/form/security.action",
			path : "/options/security.action",
			name : ""
		},
		change : {
			link : "/user/person/change/security.action"
		}
	},
	beta : {
		main : {
			link : "/person/beta.action",
			path : "/person/beta/content/form.action",
			name : ""
		},
		accept : {
			link : "/person/beta/accept.action"
		}
	},
	card : {
		list : {
			main : {
				link : "/qiwicard/content/list.action",
				path : "/cards/main.action"
			}
		}
	},
    megafon : {
        primary : {
            
            link : "/user/megafon/content/mainbalance.action"
        }
    },
	account : {
		menu : {
			link : "/person/account/leftmenu.action"
		},
		list : {
			main : {
				main : {
					link : "/user/person/account/list.action",
					path : "/settings/account/main.action",
					name : ""
				}
			},
			qiwi : {
				primary : {
					link : "/user/person/account/setmain.action"
				}
			},
			card : {
				update : {
					link : "/user/card/save.action"
				},
				remove : {
					link : "/user/card/remove.action",
					confirm : "Вы уверены, что хотите удалить карту?"
				}
			},
			webmoney : {
				remove : {
					link : "/user/webmoney/bind/reject.action",
					confirm : "Вы уверены, что хотите удалить привязанный кошелек Webmoney?"
				}
			}
		},
		create : {
			main : {
				link : "/user/person/account/form.action",
				path : "/settings/account/form.action",
				name : ""
			},
			create : {
				link : "/user/person/account/create.action"
			}
		},
		transfer : {
			main : {
				link : "/user/person/account/transfer.action",
				path : "/settings/account/transfer.action",
				name : ""
			},
			state : {
				link : "/user/person/account/state.action",
				path : "/person/account/state.action",
				name : ""
			},
			protectedstate : {
				link : "/user/person/account/state.action",
				path : "/person/account/state.action",
				name : ""
			}
		},
		identification : {
			main : {
				link : "/page/content-no-wrap.tile?name=pnp.settings.account.identification.new",
				path : "/settings/account/identification.action",
				name : ""
			},
			list : {
				link : "/user/person/account/identification/list.action",
				path : "/settings/account/identification/list.action",
				name : ""
			},
			confirm : {
				link : "/user/person/account/identification/confirm.action",
				path : "/settings/account/identification/confirm.action",
				name : ""
			},
			success : {
				link : "/user/person/account/identification/success.action",
				path : "/settings/account/identification/success.action",
				name : ""
			},
			removed : {
				link : "/user/person/account/identification/failure.action",
				path : "/settings/account/identification/failure.action",
				name : ""
			},
			identification : {
				link : "/user/person/account/identification/identify.action"
			},
			"delete" : {
				link : "/user/person/account/identification/delete.action",
				confirm : "Вы уверены, что хотите удалить запрос?"
			}
        },
		information : {
			main : {
				link : "/user/person/information/form.action"
			},
			old : {
				main:    {
					link: "/user/person/information/form-old.action"
				},
				save:    {
					link: "/user/person/information/save.action"
				},
				success: {
					link: "/user/person/information/success.action"
				}
			}
		}
	},
	email : {
		create : {
			form : {
				send : {
					link : "/user/person/email/create/send.action"
				},
				main : {
					link : "/user/person/email/content/form.action",
					path : "/settings/options/security/email.action"
				},
				success : {
					link : "/person/email/content/sendsuccess.action",
					path : "/settings/options/security/email/waited.action"
				}
			},
			confirm : {
				send : {
					link : "/user/person/email/content/message.action"
				},
				main : {
					link : "/person/email/create/content/confirm.action",
					path : "/settings/options/security/email/create.action"
				},
				success : {
					link : "/user/person/email/content/confirmed.action",
					path : "/settings/options/security/email/success.action"
				}
			}
		},
		remove : {
			form : {
				send : {
					link : "/user/person/email/remove/send.action"
				},
				main : {
					link : "/user/person/email/content/form.action",
					path : "/settings/options/security/email.action"
				},
				success : {
					link : "/person/email/content/sendsuccess.action",
					path : "/settings/options/security/email/waited.action"
				}
			},
			confirm : {
				send : {
					link : "/user/person/email/content/message.action"
				},
				main : {
					link : "/user/person/email/remove/confirm.action",
					path : "/settings/options/security/email/remove.action"
				},
				success : {
					link : "/user/person/email/content/confirmed.action",
					path : "/settings/options/security/email/success.action"
				}
			}
		}
	},
	emergency : {
		answer : {
			link : "/infoblock/emergency/answer.action"
		},
		classic : {
			link : "/temp/classicview.action"
		}
	},
    social : {
        provider : {
            list : {
                main : {
                    link : "/user/person/social/providers/list.action",
                    tmpl : "oauth.providers.list"
                },
                bind : {
                    link : "/user/person/social/providers/bind.action"
                },
                unbind : {
                    link : "/user/person/social/providers/unbind.action",
                    confirm : "Вы уверены, что хотите удалить привязку?"
                }
            }
        }
    }
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.provider = {
	search : {
		suggestion : "Все не то? Попробуй поиск!"
	},
	tabs : {
		menu : {
			link : "/provider/menutab.action"
		}
	},
	popular : {
        popular : "Популярные",
        sorted : "Все",
        menu : {
			main : {
				link : "/provider/topmenu.action"
			}
		}
	},
	tree : {
		main : {
			link : "/provider/categorylist.tile",
			path : "/payment/list.action"
		},
		menu : {
			main : {
				link : "/provider/treemenu.action"
			}
		},
		list : {
			main : {
				
				
				
				link : "/provider/content/category.action",
				path : "/payment/category.action",
				name : ""
			}
		}
	},
	operators : {
		list : {
			main : {
				link : "/provider/json/phoneoperators.action",
				path : "/provider/phoneoperators.action",
				tmpl : "provider.list",
				name : ""
			}
		}
	},
	info : {
		main : {
			link : "/provider/info/additional.action",
			path : "/provider/info/main.action"
		},
		additional : {
			link : "/provider/info/content/additional.action"
		}
	},
	provider : {
		redirect : {
			link : "/provider/redirect.action"
		},
		top : {
			link : "/provider/topmenu.action"
		}
	}

};


if (typeof settings == "undefined") {
	settings = {};
}

settings.qiwicard = {
	tabs : {
		menu : {
			link : "/qiwicard/tab.action"
		}
	},
	menu : {
		main : {
			link : "/qiwicard/menu.action"
		}
	}
};

settings.qvc = {
	menu : {
		main : {
			link : "/qvc/leftmenu.action"
		}
	},
	create : {
		main : {
			link : "/qvc/create/content/info.action",
			path : "/qvc/create/info.action",
			name : ""
		},
		form : {
			link : "/user/qvc/create/form.action",
			path : "/qvc/create/form.action",
			name : ""
		},
	   	create : {
			link : "/user/qvc/create/buy.action"
		},
		success : {
			link : "/user/qvc/create/success.action",
			path : "/qvc/create/success.action",
			name : ""
		}
	},
	recreate : {
		main : {
			link : "/qvc/recreate/content/info.action",
			path : "/qvc/recreate/info.action",
			name : ""
		},
		form : {
			link : "/user/qvc/recreate/form.action",
			path : "/qvc/recreate/form.action",
			name : ""
		},
		recreate : {
			link : "/user/qvc/recreate/buy.action"
		},
		success : {
			link : "/user/qvc/create/success.action",
			path : "/qvc/create/success.action",
			name : ""
		}
	},
	details : {
		main : {
			link : "/user/qvc/content/details.action",
			path : "/qvc/details.action",
			name : ""
		},
		details : {
			link : "/user/qvc/details.action"
		}
	},
	info : {
		main : {
			link : "/user/qvc/content/info.action",
			path : "/qvc/main.action",
			name : ""
		}
	},
	help : {
		main : {
			link : "/page/content.tile?name=card.qvc.help",
			path : "/qvc/help.action",
			name : ""
		}
	},
	remove : {
		main : {
			link : "/user/qvc/content/remove.action",
			path : "/qvc/remove.action",
			name : ""
		},
		remove : {
			link : "/user/qvc/remove.action",
			confirm : "Вы уверены, что хотите закрыть QVC?"
		}
	},
	report : {
		list : {
			main : {
				link : "/user/qvc/content/reports.action",
				path : "\/qvc\/reports.action",
				name : ""
			}
		}
	}
};

settings.qvv = {
	menu : {
		main : {
			link : "/qvv/leftmenu.action"
		}
	},
	report : {
		list : {
			main : {
				link : "/user/qvv/content/reports.action",
				path : "/qvv/reports.action",
				name : ""
			}
		}
	},
	info : {
		main : {
			link : "/user/qvv/content/info.action",
			path : "/qvv/info.action",
			name : ""
		}
	},
	help : {
		main : {
			link : "/page/content.tile?name=card.qvv.help",
			path : "/qvv/help.action",
			name : ""
		}
	},
	transfer : {
		main : {
			link : "/user/qvv/transfer/content/form.action",
			path : "/qvv/transfer/form.action",
			name : ""
		},
		success : {
			link : "/user/qvv/transfer/content/success.action",
			path : "/qvv/transfer/success.action",
			name : ""
		},
		transfer : {
			link : "/user/qvv/transfer/content/transfer.action"
		}
   	},
	resend : {
		main : {
			link : "/user/qvv/content/details.action",
			path : "/qvv/details.action",
			name : ""
		},
		resend : {
			link : "/user/qvv/resend/content/resend.action"
   		}
	},
	recharge : {
		main : {
			link : "/user/qvv/recharge/content/form.action",
			path : "/qvv/recharge/form.action",
			name : ""
		}
	},
	create : {
		main : {
			link : "/qvv/content/buy.action",
			path : "/qvv/buy.action",
			name : ""
		}
	}
};

settings.qvp = {
	menu : {
		main : {
			link : "/qvp/leftmenu.action"
		}
	},
	create : {
		main : {
			link : "/qvp/create/content/description.action",
			path : "/qvp/create/description.action",
			name : ""
		},
		form : {
			link : "/user/qvp/create/content/form.action",
			path : "/qvp/create/form.action",
			name : ""
		},
		delivery : {
			link : "/user/qvp/create/content/delivery.action"
		},
		region : {
			link : "/user/qvp/create/content/regions.action"
		}
	},
	activate : {
		main : {
			link : "/user/qvp/activate/content/main.action",
			path : "/qvp/activate/main.action",
			name : ""
		},
		activate : {
			link : "/user/qvp/activate/content/make.action"
		},
		success : {
			link : "/user/qvp/activate/content/success.action",
			path : "/qvp/activate/success.action",
			name : ""
		},
		failure : {
			link : "/user/qvp/activate/content/failure.action",
			path : "/qvp/activate/failure.action",
			name : ""
		}
	},
	info: {
		main : {
			link : "/user/qvp/content/info.action",
			path : "/qvp/info.action",
			name : ""
		}
	},
	help : {
		main : {
			link : "/page/content.tile?name=card.qvp.help",
			path : "/qvp/help.action",
			name : ""
		}
	},
	report : {
		list : {
			main : {
				link : "/user/qvp/content/reports.action",
				path : "/qvp/reports.action",
				name : ""
			}
		}
	},
	close : {
		main : {
			link : "/user/qvp/close/content/main.action",
			path : "/qvp/close/main.action",
			name : ""
		},
		close : {
			link : "/user/qvp/close/content/make.action",
			confirm : "Вы уверены, что хотите закрыть QVP?"
		},
		success : {
			link : "/user/qvp/close/content/success.action",
			path : "/qvp/close/success.action",
			name : ""
		},
		failure : {
			link : "/user/qvp/close/content/failure.action",
			path : "/qvp/close/failure.action",
			name : ""
		}
	},
	rename : {
		main : {
			link : "/user/qvp/rename/content/main.action",
			path : "/qvp/rename/main.action",
			name : ""
		},
		rename : {
			link : "/user/qvp/rename/content/make.action"
		},
		success : {
			link : "/user/qvp/rename/content/success.action",
			path : "/qvp/rename/success.action",
			name : ""
		},
		failure : {
			link : "/user/qvp/rename/content/failure.action",
			path : "/qvp/rename/failure.action",
			name : ""
		}
	},
	pin : {
		main : {
			link : "/user/qvp/pin/content/main.action",
			path : "/qvp/pin/main.action",
			name : ""
		},
		pin : {
			link : "/user/qvp/pin/content/make.action"
		},
		success : {
			link : "/user/qvp/pin/content/success.action",
			path : "/qvp/pin/success.action",
			name : ""
		},
		failure : {
			link : "/user/qvp/pin/content/failure.action",
			path : "/qvp/pin/failure.action",
			name : ""
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.replenish = {
	all : {
		main : {
			link : "/replenish/all/content.action",
			path : "/replenish/all.action",
			name : "Пополнение"
		},
		filters : {
			link : "/replenish/method/filters.action"
		},
		methods : {
			link : "/replenish/method/list.action"
		},
		result : {
			link : "/replenish/method/result.action"
		},
		instruction : {
			link : "/replenish/method/instruction.action"
		}
	},
	tools : {
		main : {
			link : "/replenish/tool/content.action",
			path : "/replenish/tool.action",
			name : "Пополнение"
		},
		popular : {
			link : "/replenish/method/popular.action"
		},
		tools : {
			link : "/replenish/tool/list.action"
		}
	},
	map : {
		main : {
            path : "/replenish/map.action"
		},
		points : {
			link : "/replenish/map/list.action"
		},
		information : {
			link : "/replenish/map/information.action"
		},
		icon : {
			terminal : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/terminal.png"
			},
			terminal2x : {
				link : "https://static.qiwi.com/img/qiwi_com/ui/replenish/map/icon/terminal2.png"
			},
			card : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/card.png"
			},
			card2x : {
				link : "https://static.qiwi.com/img/qiwi_com/ui/replenish/map/icon/card2.png"
			},
			office : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/office.png"
			},
			shop : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/shop.png"
			},
			money : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/money.png"
			},
			money2x : {
				link : "https://static.qiwi.com/img/qiwi_com/ui/replenish/map/icon/money2.png"
			},
			moneycard : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/moneycard.png"
			},
			cluster : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/cluster.png"
			},
			shadow : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/shadow.png"
			},
			none : {
				link : "https://static.qiwi.com/img/visa_qiwi_com/ui/replenish/map/icon/none.png"
			}
		}
	},
	terminals : {
		main : {
			link : "/replenish/map/terminals.action",
			path : "/replenish/terminals.action"
		}
	},
	instruction : {
		print : {
			link : "/replenish/method/print.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.report = {
	list : {
		main : {
			link : "/user/report/list.action",
			path : "/report/list.action",
			name : ""
		},
		extra : {
			link : "/user/report/extras.action"
		}
	},
	menu : {
		link : "/report/leftmenu.action"
	},
	courses : {
		main: {
			link: "/report/content/courses.action",
			path: "/report/courses.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.search = {
	list : {
		link : "/search/results/json.action",
		path : "/search/results.action",
		tmpl : "search.list",
		name : ""
	},
	"public" : {
		link : "/search/results/public/json.action",
		path : "/search/public/results.action",
		tmpl : "pub.search.list",
		name : ""
	},
	autocomplete : {
		link : "/search/autocomplete.action"
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.shell = {
	ajax: {
		0: "ОК",
		1: "Ошибка",
		2: "Ошибка",
		3: "Требуется авторизация",
		4: "Требуется подтверждение",
		5: "Требуется подтверждение",
		6: "Срок действия пароля истек",
		7: "Требуется токен",
		success: "Успех",
		timeout: "Время соединения истекло. Проверьте подключение к сети и попробуйте войти еще раз",
		error: "Ошибка соединения. Проверьте подключение к сети и попробуйте войти еще раз",
		abort: "Запрос отменен",
		parsererror: "Синтаксическая ошибка",
		"Internal XMLHttpRequest Error": "Ошибка соединения. Проверьте подключение к сети и попробуйте войти еще раз",
		"XMLHttpRequest timeout": "Время соединения истекло. Проверьте подключение к сети и попробуйте войти еще раз"
	},
	"disable-third-party-cookies": "У Вас выключено сохранение со сторонних сайтов локальных данных (cookies) в браузере. Включите их в настройках.",
	http: {
		code: {
			100: "100",
			101: "101",
			102: "102",
			105: "105",
			200: "200",
			201: "201",
			202: "202",
			203: "203",
			204: "204",
			205: "205",
			206: "206",
			207: "207",
			226: "226",
			300: "300",
			301: "301",
			302: "302",
			303: "303",
			304: "304",
			305: "305",
			306: "306",
			307: "307",
			400: "400",
			401: "Попробуйте позднее",
			402: "402",
			403: "Запрещен доступ",
			404: "Страница не найдена",
			405: "405",
			406: "406",
			407: "407",
			408: "408",
			409: "409",
			410: "410",
			411: "411",
			412: "412",
			413: "Размер запроса слишком велик",
			414: "413",
			415: "414",
			416: "416",
			417: "417",
			418: "418",
			422: "422",
			423: "423",
			424: "424",
			425: "425",
			426: "426",
			428: "428",
			429: "429",
			431: "431",
			434: "434",
			449: "449",
			451: "451",
			456: "456",
			499: "499",
			500: "Попробуйте позднее",
			501: "501",
			502: "502",
			503: "503",
			504: "Сервер временно недоступен",
			505: "505",
			506: "506",
			507: "507",
			508: "508",
			509: "509",
			510: "510",
			511: "undefined"
		}
	},
	datepicker: {
		closeText: "Закрыть",
		prevText: "&#x3c;Пред",
		nextText: "След&#x3e;",
		currentText: "Сегодня",
		monthNames: ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"],
		monthNamesShort: ["Янв", "Фев", "Мар", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"],
		dayNames: ["воскресенье", "понедельник", "вторник", "среда", "четверг", "пятница", "суббота"],
		dayNamesShort: ["вск", "пнд", "втр", "срд", "чтв", "птн", "сбт"],
		dayNamesMin: ["Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"],
		weekHeader: "Нед",
		dateFormat: "dd.mm.yy",
		firstDay: 1,
		isRTL: false,
		showMonthAfterYear: false,
		yearSuffix: " "
	},
	validate: {
		required: "Это поле необходимо заполнить",
		remote: "Пожалуйста, введите правильное значение.",
		email: "Пожалуйста, введите корректный адрес электронной почты.",
		url: "Пожалуйста, введите корректный URL.",
		date: "Пожалуйста, введите корректную дату.",
		dateISO: "Пожалуйста, введите корректную дату в формате ISO.",
		number: "Пожалуйста, введите число.",
		digits: "Пожалуйста, вводите только цифры.",
		creditcard: "Пожалуйста, введите правильный номер кредитной карты.",
		equalTo: "Пожалуйста, введите такое же значение ещё раз.",
		accept: "Пожалуйста, выберите файл с правильным расширением.",
		maxlength: "Пожалуйста, введите не больше {0} символов.",
		minlength: "Пожалуйста, введите не меньше {0} символов.",
		rangelength: "Пожалуйста, введите значение длиной от {0} до {1} символов.",
		range: "Пожалуйста, введите число от {0} до {1}.",
		max: "Пожалуйста, введите число, меньшее или равное {0}.",
		min: "Пожалуйста, введите число, большее или равное {0}.",
        regexp: "Пожалуйста, введите корректное значение.",
        inn: "Пожалуйста, введите правильный ИНН.",
        snils: "Пожалуйста, введите правильный СНИЛС.",
        passport: "Пожалуйста, введите правильные паспортные данные.",
        agerange: "Вам должно быть больше 18 лет.",
        registered: "Номер не зарегистрирован",
        ping: "Пожалуйста, введите корректный адрес сайта. Сайт должен быть доступен.",
		passworded: "Пожалуйста, введите значение, которое содержит как минимум один символ верхнего регистра, один нижнего регистра и одну цифру.",
		error: "Это поле необходимо заполнить"
	},
    currency: {
        398: {
            code: "KZT",
            ititle: "тенге",
            ftitle: "тиын",
            ishort: "тенге",
            fshort: "тиын"
        },
        643: {
            code: "RUB",
            ititle: "руб.",
            ftitle: "коп.",
            ishort: "руб.",
            fshort: "коп."
        },
        498: {
            code: "MDL",
            ititle: "лей",
            ftitle: "бань",
            ishort: "лей",
            fshort: "бань"
        },
        756: {
            code: "CHE",
            ititle: "undefined",
            ftitle: "undefined",
            ishort: "undefined",
            fshort: "undefined"
        },
        840: {
            code: "USD",
            ititle: "долл.",
            ftitle: "цент.",
            ishort: "долл.",
            fshort: "цент."
        },
        972: {
            code: "TJS",
            ititle: "сом.",
            ftitle: "дирам",
            ishort: "сом.",
            fshort: "дирам"
        },
        978: {
            code: "EUR",
            ititle: "евро",
            ftitle: "цент.",
            ishort: "евро",
            fshort: "цент."
        },
        980: {
            code: "UAH",
            ititle: "грн.",
            ftitle: "коп.",
            ishort: "грн.",
            fshort: "коп."
        }
    },
	dialog: {
		title: {
			error: "Ошибка",
			info: "Информация",
			attention: "Внимание"
		},
		button: {
			"continue": "Продолжить",
			close: "Закрыть",
			yes: "Да",
			no: "Нет"
		}
	},
	request: {
		message: "Загрузка"
	},
	status: {
		success: "https:\/\/static.qiwi.com\/img\/visa_qiwi_com\/ui\/statuses\/success.png",
		error: "https:\/\/static.qiwi.com\/img\/visa_qiwi_com\/ui\/statuses\/error.png"
	},
	paging: {
		sizes: {
			show: "Показывать",
			onpage: "на странице"
		}
	},
	selfxss: {
		stop: "Остановитесь!",
		warn: "Эта функция браузера предназначена для разработчиков. Если кто-то сказал вам скопировать и вставить что-то здесь, чтобы увеличить вдвое ваш баланс, или взломать чужой кошелек, это мошенники. Выполнив эти действия, вы предоставите им доступ к своему Qiwi кошельку."
	}
};

settings.goog = {
	"Today": "Сегодня",
	"None": "Закрыть",
    "Please enter a value": "Это поле необходимо заполнить",
    "Please enter a valid value": "Пожалуйста, введите корректное значение.",
    "Commission": "Комиссия",
    "Additional commission": "Дополнительная комиссия",
    "not less": "не менее",
    "not more": "не более",
    "Year": "Год",
    "Month": "Месяц",
    "Minimum payment": "Минимальная сумма платежа",
    "Maximum payment": "Максимальная сумма платежа с учетом комиссии",
    "Payment information": "Информация о платеже",
    "SMS resend status": "Повторно запросить код можно будет через %s сек.",
    "Incorrect amount": "Некорректная сумма.",
    "Amount less than the minimum": "Сумма меньше минимальной",
    "Amount greater than the maximum": "Сумма больше максимальной",
    "Sms commission": " + {0} взимает {1} {2} (с учетом НДС) за SMS об успешном проведении платежа",
    "Conversion rate": "Курс конвертации банка",
    "Adding to favorites": "Добавление в избранное",
    "Close": "Закрыть",
	"Unlinked card": "Другой картой",
    "Acquiring pan tooltip" : "\u003Cp\u003E\u003Cstrong\u003EЭта карта уже привязана к вашему Visa QIWI Кошельку.\u003C\/strong\u003E\u003C\/p\u003E\n\u003Cp\u003EВыберите ее среди способов оплаты – и вам не придется вводить данные карты.\u003C\/p\u003E",
    "Acquiring cvv tooltip" : "\u003Cp\u003EКод CVV \/ CVC указан\u003Cbr \/\u003E на обратной стороне карты\u003C\/p\u003E",
	"Acquiring save tooltip" : "\u003Cp\u003E\u003Cstrong\u003EДля быстрых и безопасных платежей\u003C\/strong\u003E\u003C\/p\u003E\n\u003Cp\u003EВаша карта будет привязана к Visa QIWI Wallet и вам не придется вводить данные карты для последующих платежей\u003C\/p\u003E\n\u003Cp\u003E\u003Ca href=\"\/support\/faq\/binding-of-bank-card.action\" target=\"_blank\"\u003EУзнать подробнее о привязке карты\u003C\/a\u003E\u003C\/p\u003E",
	"Schedule title" : "Запланировать",
    "Schedule open" : "Запланировать?",
	"Schedule close" : "Отменить",
	"Schedule notify day label" : "дней до платежа",
	"Schedule notify cost" : "(Стоимость 1,5 р.)"
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.sinap = {
    payment : {
        success : {
            link : "/payment/form/success.action"
        }
    },
    information : {
        sources : {
            account : {
                nosource: "Платежи с баланса Visa QIWI Wallet на данного провайдера запрещены.",
                onesource: "Платежи с баланса Visa QIWI Wallet на данного провайдера запрещены.",
                fewsources: "У вас существует несколько источников средств для совершения платежа. Чтобы перейти к оплате, выберите удобный источник."
            },
            card : {
                nosource: "Для оплаты банковской картой вам необходимо ее \u003Ca href=\"\/card\/content.action\" target=\"_blank\"\u003Eзарегистрировать\u003C\/a\u003E. Выберите один из доступных способов оплаты.",
                onesource: "Оплата с банковской карты недоступна без \u003Ca href=\"\/card\/content.action\" target=\"_blank\"\u003Eрегистрации\u003C\/a\u003E. По умолчанию для платежа будут использованы средства из Visa QIWI Wallet.",
                fewsources: "У вас существует несколько источников средств для совершения платежа. Чтобы перейти к оплате, выберите удобный источник."
            },
            mobile : {
                nosource: "Данный провайдер не поддерживает вашего оператора сотовой связи. Выберите один из доступных способов оплаты.",
                onesource: "Оплата со счета телефона недоступна. По умолчанию для платежа будут использованы средства из Visa QIWI Wallet.",
                fewsources: "У вас существует несколько источников средств для совершения платежа. Чтобы перейти к оплате, выберите удобный источник."
            }
        }
    },
    provider : {
        99 : {
            header : "\u003Cp\u003EПереводы Visa QIWI Кошелька позволяют отправить деньги друзьям и знакомым: достаточно знать номер мобильного телефона.\u003C\/p\u003E\u003Cp\u003E&nbsp;\u003C\/p\u003E\u003Cp\u003EВсё просто:\u003C\/p\u003E\u003Cp\u003E&nbsp;\u003C\/p\u003E\u003Cp\u003E1. Заполните форму\u003C\/p\u003E",
            footer : "\u003Cp\u003E2. Visa QIWI Кошелек направит получателю SMS-уведомление о поступлении перевода.\u003C\/p\u003E\n\u003Cp\u003E&nbsp;\u003C\/p\u003E\n\u003Cp\u003E3. Чтобы воспользоваться деньгами получателю достаточно войти в Visa QIWI Кошелек или зарегистрироваться.\u003C\/p\u003E\n\u003Cp\u003E&nbsp;\u003C\/p\u003E\n\u003Cp\u003EПереводы осуществляются через КИВИ Банк (АО).\u003C\/p\u003E\n\u003Cp\u003EУслуга оказывается на основании \u003Ca target=\"_blank\" href=\"\/page\/file.action?id=2023\"\u003EОферты\u003C\/a\u003E.\u003C\/p\u003E"
        }
    },
    refresh : {
        identificationRequest : "Запрос статуса идентификации",
        identificationResult : "Статус идентификации позволяет совершить платеж"
    },
    state : {
        form : {
            controls : {
                form : [
                    {
                        text : "Оплатить",
                        name : "confirm",
                        type : "orange-button"
                    },
                    {
                        text : "В избранное",
                        name : "favorite",
                        type : "favorite-button"
                    },
                    {
                        text : "Запланировать",
                        name : "schedule",
                        type : "schedule-button"
                    }
                ],
                open : [
                    {
                        text : "Оплатить",
                        name : "confirm",
                        type : "orange-button"
                    },
                    {
                        text : "Запланировать",
                        name : "schedule",
                        type : "schedule-button"
                    }
                ],
                edit : [
                    {
                        text : "Сохранить изменения",
                        name : "favorite",
                        type : "orange-button"
                    }
                ],
                save : [
                    {
                        text : "Сохранить изменения",
                        name : "favorite",
                        type : "orange-button"
                    }
                ]
            }
        },
        confirm : {
            controls : [
                {
                    text : "Подтвердить",
                    name : "pay",
                    type : "orange-button"
                },
                {
                    text : "← Вернуться",
                    name : "form",
                    type : "link-button"
                }
            ]
        },
        offer : {
            controls : [
                {
                    text : "Подтвердить",
                    name : "confirm",
                    type : "orange-button"
                },
                {
                    text : "← Вернуться",
                    name : "form",
                    type : "link-button"
                }
            ],
            condition: {
                "switch": {
                    title: "Я согласен с условиями",
                    offValue: false,
                    onValue: true
                },
                link : {
                    text : "Оферты",
                    href: ""
                },
                error : "Согласитесь с условиями Оферты"
            },
            list : {
                ru: {
                    link: "/offer.action",
                    currencies: [643, 840, 978],
                    signed: false
                },
                kz: {
                    link: "/offerkz.action",
                    currencies: [398],
                    signed: false
                }
            }
        },
        confirmsources : {
            controls : [
                {
                    text : "Подтвердить",
                    name : "confirm",
                    type : "orange-button"
                },
                {
                    text : "← Вернуться",
                    name : "form",
                    type : "link-button"
                }
            ]
        },
        favorite : {
            controls : {
                form : [
                    {
                        text : "Сохранить изменения",
                        name : "save",
                        type : "orange-button"
                    },
                    {
                        text : "← Вернуться",
                        name : "form",
                        type : "link-button"
                    }
                ],
                open : [
                    {
                        text : "Сохранить изменения",
                        name : "save",
                        type : "orange-button"
                    },
                    {
                        text : "← Вернуться",
                        name : "form",
                        type : "link-button"
                    }
                ],
                edit : [
                    {
                        text : "Сохранить изменения",
                        name : "save",
                        type : "orange-button"
                    },
                    {
                        text : "← Вернуться",
                        name : "form",
                        type : "link-button"
                    }
                ],
                save : [
                    {
                        text : "Сохранить изменения",
                        name : "save",
                        type : "orange-button"
                    },
                    {
                        text : "← Вернуться",
                        name : "form",
                        type : "link-button"
                    }
                ]
            },
            title : {
                name: "title",
                validator : {
                    message : "Это поле необходимо заполнить",
                    type : "required"
                },
                view: {
                    title : "Название",
                    widget: {
                        type: "text"
                    }
                }
            },
            schedule : {
                interval : {
                    name: "interval",
                    validator: {
                        type: "predicate",
                        predicate: {
                            type: "regex",
                            pattern: "^.+$"
                        },
                        message: "Это поле необходимо заполнить"
                    },
                    view: {
                        title: null,
                        widget: {
                            type: "radio",
                            choices: [{
                                value : "ONCE",
                                title : "Один раз"
                            }, {
                                value : "WEEK",
                                title : "Каждую неделю"
                            }, {
                                value : "MONTH",
                                title : "Каждый месяц"
                            }],
                            orientation: "horizontal"
                        }
                    }
                },
                start : {
                    date : {
                        name: "startDate",
                        validator: {
                            type: "predicate",
                            predicate: {
                                type: "regex",
                                pattern: "^\\d{2}.\\d{2}.\\d{4}$"
                            },
                            message: "Пожалуйста, введите корректную дату."
                        },
                        view: {
                            title: "Дата начала",
                            widget: {
                                type: "date",
                                format: "dd.MM.yyyy",
                                allowsFutureDate: true
                            }
                        }
                    },
                    time : {
                        name: "startTime",
                        validator: {
                            type: "successful"
                        },
                        view: {
                            title: "Время",
                            widget: {
                                type: "text",
                                mask: "hh:mm:ss"
                            }
                        }
                    }
                },
                weekly : {
                    name: "weekly",
                    validator: {
                        type: "predicate",
                        predicate: {
                            type: "regex",
                            pattern: "^.+$"
                        },
                        message: "Это поле необходимо заполнить"
                    },
                    view: {
                        title: null,
                        widget: {
                            type: "checkbox",
                            choices: [{
                                value : 1,
                                title : "понедельник"
                            }, {
                                value : 2,
                                title : "вторник"
                            }, {
                                value : 3,
                                title : "среда"
                            }, {
                                value : 4,
                                title : "четверг"
                            }, {
                                value : 5,
                                title : "пятница"
                            }, {
                                value : 6,
                                title : "суббота"
                            }, {
                                value : 7,
                                title : "воскресенье"
                            }],
                            orientation: "horizontal"
                        }
                    }
                },
                monthly : {
                    name: "weekly",
                    validator: {
                        type: "predicate",
                        predicate: {
                            type: "regex",
                            pattern: "^.+$"
                        },
                        message: "Это поле необходимо заполнить"
                    },
                    view: {
                        title: null,
                        widget: {
                            type: "checkbox",
                            choices: [{
                                value : 1,
                                title : "01"
                            }, {
                                value : 2,
                                title : "02"
                            }, {
                                value : 3,
                                title : "03"
                            }, {
                                value : 4,
                                title : "04"
                            }, {
                                value : 5,
                                title : "05"
                            }, {
                                value : 6,
                                title : "06"
                            }, {
                                value : 7,
                                title : "07"
                            }, {
                                value : 8,
                                title : "08"
                            }, {
                                value : 9,
                                title : "09"
                            }, {
                                value : 10,
                                title : "10"
                            }, {
                                value : 11,
                                title : "11"
                            }, {
                                value : 12,
                                title : "12"
                            }, {
                                value : 13,
                                title : "13"
                            }, {
                                value : 14,
                                title : "14"
                            }, {
                                value : 15,
                                title : "15"
                            }, {
                                value : 16,
                                title : "16"
                            }, {
                                value : 17,
                                title : "17"
                            }, {
                                value : 18,
                                title : "18"
                            }, {
                                value : 19,
                                title : "19"
                            }, {
                                value : 20,
                                title : "20"
                            }, {
                                value : 21,
                                title : "21"
                            }, {
                                value : 22,
                                title : "22"
                            }, {
                                value : 23,
                                title : "23"
                            }, {
                                value : 24,
                                title : "24"
                            }, {
                                value : 25,
                                title : "25"
                            }, {
                                value : 26,
                                title : "26"
                            }, {
                                value : 27,
                                title : "27"
                            }, {
                                value : 28,
                                title : "28"
                            }, {
                                value : 31,
                                title : "Последний день месяца"
                            }],
                            orientation: "horizontal"
                        }
                    }
                },
                autopay : {
                    title : "Платить автоматически",
                    offValue : false,
                    onValue : true
                },
                notify : {
                    "switch" : {
                        title : "Предупреждать по SMS за",
                        offValue : false,
                        onValue : true
                    },
                    day : {
                        input : {}
                    }
                }
            }
        },
        favoritesources : {
            controls : [
                {
                    text : "Подтвердить",
                    name : "favorite",
                    type : "orange-button"
                },
                {
                    text : "← Вернуться",
                    name : "form",
                    type : "link-button"
                }
            ]
        },
        pay: {
            smsform : {
                header : "Для подтверждения платежа вам отправлено SMS с одноразовым кодом",
                code : {
                    name: "code",
                    title: "Введите код",
                    validator: {
                        type : "predicate",
                        predicate : {
                            type: "regex",
                            pattern: "^\\d{1,8}$"
                        },
                        message : "Это поле необходимо заполнить"
                    }
                },
                submit : "Подтвердить",
                wait: "Ожидайте звонка.",
                resend : {
                    sms: "Прислать код повторно",
                    phone: {
                        title: "Прослушать код по телефону",
                        description: "Мы позвоним вам и продиктуем код"
                    }
                }
            },
            external : {
                title : "Подтверждение платежа с банковской карты",
                description : "Для подтверждения платежа необходимо перейти на страницу банка",
                submit: "Перейти"
            }
        },
        save: {
            smsform : {
                header : "Для планирования платежа вам отправлено SMS с одноразовым кодом",
                code : {
                    name: "code",
                    title: "Введите код",
                    validator: {
                        type : "predicate",
                        predicate : {
                            type: "regex",
                            pattern: "^\\d{1,8}$"
                        },
                        message : "Это поле необходимо заполнить"
                    }
                },
                submit : "Подтвердить",
                wait: "Ожидайте звонка.",
                resend : {
                    sms: "Прислать код повторно",
                    phone: {
                        title: "Прослушать код по телефону",
                        description: "Мы позвоним вам и продиктуем код"
                    }
                }
            }
        }
    },
    field : {
        source: {
            name: "source",
            title: "Способ оплаты",
            items: [],
            types: {
                account: {
                    type: "Account",
                    title: "Visa QIWI Кошелек",
                    description: "Вы можете оплатить эту услугу со счета своего Visa QIWI Кошелька после \u003Ca href=\"\/register\/form.action\" target=\"_blank\"\u003Eрегистрации\u003C\/a\u003E"
                },
                card: {
                    type: "Card",
                    title: "Банковские карты",
                    description: "\u003Ca href=\"\/card\/content.action\" target=\"_blank\"\u003EЗарегистрируйте\u003C\/a\u003E свою банковскую карту в Visa QIWI Кошельке и совершайте безопасные платежи с ее счета\n"
                },
                mobile: {
                    type: "Mobile",
                    title: "Баланс сотового телефона",
                    description: "Зарегистрировавшись в Visa QIWI Кошельке, вы легко сможете оплатить покупку со счета МТС, Билайн, МегаФон или Tele2"
                },
                terminal: {
                    type: "Terminal",
                    title: "Терминалы",
                    description: "Найдите ближайший QIWI Терминал \u003Ca href=\"\/replenish\/map.action?method=1&owners=1&owners=16&owners=24\" target=\"_blank\"\u003Eна карте\u003C\/a\u003E и оплатите покупку наличными"
                }
            }
        },
        amount: {
            name: "sum",
            title: "Сумма",
            total: {
                title: "Сумма с учетом комиссии"
            },
            validator: {
                message: "Это поле необходимо заполнить",
                type: "required"
            }
        },
        comment: {
            name : "comment",
            validator : {
                type : "predicate",
                predicate : {
                    type : "regex",
                    pattern : "^.{0,255}$"
                },
                message : "Пожалуйста, введите корректное значение."
            },
            view : {
                title : "Комментарий (необязательно)",
                widget : {
                    type : "text"
                }
            }
        }
    },
    acquiring : {
        card : {
            pan : {
                name : "card.pan",
                validator : {
                    type : "predicate",
                    predicate : {
                        type : "regex",
                        pattern : "^[0-9]{4}\\s[0-9]{4}\\s[0-9]{4}\\s[0-9]{4}\\s*$|^[0-9]{4}\\s[0-9]{4}\\s[0-9]{4}\\s[0-9]{4}\\s[0-9]{1,3}\\s*$"
                    },
                    message : "Пожалуйста, введите корректное значение."
                },
                view : {
                    title : "Номер карты",
                    widget : {
                        type : "text",
                        mask: "dddd dddd dddd dddd? ddd"
                    }
                }
            },
            date : {
                name : "card.date",
                validator : {
                    type : "and",
                    children : [{
                        type: "predicate",
                        predicate: {
                            type: "regex",
                            pattern: "^\\d{6}$"
                        },
                        message: "Пожалуйста, введите корректную дату."
                    }, {
                        type: "predicate",
                        predicate: {
                            type: "greater",
                            value: new Date().toISOString().slice(0,7).replace(/-/g,"")
                        },
                        message: "Пожалуйста, введите корректную дату."
                    }]
                },
                view : {
                    title : "VALID\u003Cbr \/\u003ETHRU",
                    widget: {
                        type : "expiryDate",
                        format : "yyyyMM",
                        yearFormat : "yy",
                        yearPlaceholder : "ГГ",
                        monthFormat : "MM",
                        monthPlaceholder : "ММ"
                    }
                }
            },
            cvv : {
                name : "card.cvv",
                validator : {
                    type : "predicate",
                    predicate : {
                        type : "regex",
                        pattern : "^\\d{3}$"
                    },
                    message : "Пожалуйста, введите корректное значение."
                },
                view : {
                    title : "CVV\u003Cbr \/\u003ECVC",
                    widget : {
                        type : "text",
                        mask: "ddd"
                    }
                }
            }
        },
        save : {
            name : "save.card",
            view : {
                title: "Привязать карту к Visa QIWI Wallet",
                widget : {
                    type: "switch",
                    offValue: false,
                    onValue: true
                }
            },
            value: true
        },
        message : "Введите данные карты"
    },
    suggest : "Выберите вариант или продолжите ввод:"
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.support = {
	feedback : {
		main : {
			link : "/feedback/content/form.action",
			path : "/support/form.action"
		},
		send : {
			link : "/feedback/content/send.action",
            button: "Отправить"
		},
        form : {
            link : "/feedback/content/formproxy.action"
        },
		success : {
			link : "/feedback/content/success.action",
			path : "/support/form/success.action"
		},
		fields : {
			link : "/feedback/content/fields.action"
		}
	},
	landlords : {
		main : {
			link : "/landlords/content/form.action",
			path : "/landlords/main.action"
		},
		send : {
			link : "/landlords/content/send.action"
		},
		success : {
			link : "/landlords/content/success.action",
			path : "/landlords/success.action"
		}
	},
	check : {
		main : {
			link : "/support/check/content/form.action",
			path : "/support/check.action"
		},
		result : {
			link : "/support/check/content/result.action",
			path : "/support/check/result.action"
		}
	},
	help : {
		main : {
			link : "/support/content/help.action",
			path : "/support/help.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.transfer = {
	eggs : {
		buy : {
			main : {
				link : "/payment/form/start.action?provider=22496",
				path : "/transfer/eggs/buy.action",
				name : ""
			}
		},
		activate : {
			main : {
				link : "/user/eggs/activate/content/form.action",
				path : "/transfer/eggs/activate.action",
				name : ""
			},
			link : "/user/eggs/activate/content/activate.action",
			success : {
				link : "/user/eggs/activate/content/success.action",
				path : "/transfer/eggs/activate/success.action",
				name : ""
			},
			failure : {
				link : "/user/eggs/activate/content/failure.action",
				path : "/transfer/eggs/activate/failure.action",
				name : ""
			}
		},
		list : {
			main : {
				link : "/user/eggs/content/list.action",
				path : "/transfer/eggs/list.action",
				name : ""
			}
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.unistream = {
	calculate : {
		link : "/payment/form/calculate.action"
	},
	card : {
		link : "/unistream/content/card.action"
	},
	newrecipient : "Новый получатель"
};


if (typeof settings == "undefined") {
	settings = {};
}

settings.webmoney = {
	description : {
		link : "/webmoney/content/description.action",
		path : "/settings/account/form/webmoney.action",
		name : ""
	},
	instruction : {
		link : "/webmoney/content/instruction.action",
		path : "/webmoney/instruction.action",
		name : ""
	},
	bind : {
		status : {
			main : {
				link : "/user/webmoney/bind/status.action",
				path : "/webmoney/bind/status.action",
				name : ""
			}
		},
		incoming : {
			form : {
				link : "/user/webmoney/bind/incoming/content/form.action",
				path : "/webmoney/bind/incoming/form.action",
				name : ""
			},
			accept : {
				link : "/user/webmoney/bind/incoming/accept.action"
			}
		},
		outcoming : {
			form : {
				link : "/user/webmoney/bind/outcoming/content/form.action",
				path : "/webmoney/bind/outcoming/form.action",
				name : ""
			},
			accept : {
				link : "/user/webmoney/bind/outcoming/accept.action"
			}
		}
	},
	payments : {
		form : {
			link : "/user/webmoney/payments/form.action",
			path : "/webmoney/payments/form.action",
			name : ""
		},
		change : {
			link : "/user/webmoney/payments/change.action"
		}
	}
};


if (typeof settings == "undefined") {
	settings = {};
}

if (typeof settings.providers == "undefined") {
	settings.providers = {};
}

settings.providers.wu = {
	card : {
		link : "/wu/content/card.action"
	},
	calculate: {
		link : "/wu/content/comm.action"
	},
	offer: {
		link : "/wu/content/offer.action"
	},
	newrecipient : "Новый получатель",
	fields : {
		to_name_f : "Фамилия",
		to_name_p : "Отчество",
		from_name_f : "Фамилия",
		from_name_p : "Отчество"
	},
	fieldslat : {
		to_name_f : "Recipients maternal",
		to_name_p : "Recipients paternal",
		from_name_f : "Senders paternal",
		from_name_p : "Senders maternal"
	}
};

